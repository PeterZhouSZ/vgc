// Copyright 2018 The VGC Developers
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/vgc/vgc/blob/master/COPYRIGHT
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <vgc/dom/schema.h>

namespace vgc {
namespace dom {

Schema::Schema(core::StringId name,
               const std::map<core::StringId, BuiltInAttribute>& attributes) :
    name_(name),
    attributes_(attributes)
{

}

const Value& Schema::defaultValue(core::StringId name) const
{
    auto search = attributes_.find(name);
    if (search != attributes_.end()) {
        return search->second.defaultValue();
    } else {
        Value::invalid();
    }
}

ValueType Schema::valueType(core::StringId name) const
{
    auto search = attributes_.find(name);
    if (search != attributes_.end()) {
        return search->second.valueType();
    } else {
        ValueType::Invalid;
    }
}

} // namespace dom
} // namespace vgc

// Note:
//
// For custom attributes, instead of
//
//   data-vec2darray-pos="[]"
//
// we could have had:
//
//   data-pos="Vec2dArray([])"
//
// If we did this, then it would make sense to do it for built-in attributes too:
//
//   positions="Vec2dArray([])"
//
// Advantages:
// 1. It is a valid Python expression (useful for copy-pasting to console).
// 2. Allow attributes to be polymorphic.
//
// Inconvenients:
// 3. More verbose for built-in attributes.
// 4. Allow attributes to be polymorphic (more complexity to handle).
//
// At the end of the day, we decided that 3. was too strong of an inconvenience,
// especially for small types like "int":
//
// <vertex
//   cid="Int(42)"/>
//
// Also, what to do in case of keyframes? The following is really nice:
//
// positions="
//   1: [(0, 0), (0, 0)]
//   23: [(0, 0), (10, 0)]
//   47: [(0, 0), (10, 10)]"
//
// However, note that the cool notation above means that we should make ':' a
// reserved character, to be escaped (e.g., '\:') for strings:
//
// <text string="Two words\: hello world!">
//
// This means that the escaping character '\' itself should be reserved, so
// that if you literally want to write the two characters "\:" you can encode
// it in as "\\\:". It is a bit annoying to have another layer of escaping on
// top of XML escaping already (for &, <, >, ', and "), but there is no way
// around it if we want to be able to animate strings themselves:
//
// string="
//   1:  Hello
//   23: world!"
//
// Would this be an animated string, or a string that contains the character
// ":" two times? An alternative would be to encode the fact that those are
// keyframes in the attribute name:
//
// string-keyframes="
//   1:  Hello
//   23: world!"
//
// But this doesn't remove the need to be able to escape ':' if you want to
// animate a string who may contain the character ':', so we actually don't
// gain much and just make the syntax uglier. And in fact, having ':' a special
// VGC character allow us to define other very cool syntax, such as:
//
// <rect x="0", y="0", height="100", width="fn: 100 + 50 * cos(0.5*t)"/>
//
// And since we now have an escaping character '\', we can use it for other
// useful things, such as defining a newline via '\n' instead of having to
// insert a literal newline.
//
// Also, by default, we may want to remove all superfluous whitespaces around
// the string, so that when writing the following:
//
// string="
//   1:  Hello
//   23: world!"
//
// the animated string is "Hello" at frame 1, and "world!" at frame 23. In
// order to include the leading whitespace before "Hello", users could do:
//
// string="
//   1: \( Hello\)
//   23: world!"
//
// All of this to say that there is enough complexity within the attribute
// value itself, without having to define the type as part of the value. It
// seems cleaner and more robust to simply have the type encoded within the
// name (for custom attributes), or have it defined as part of the schema (for
// built-in attributes).
//