#include "components/render_component.h"

#include <string>

namespace platformer2d {

RenderComponent::RenderComponent(const std::string& tag,
                                 const std::string& texture_name)
    : Component(tag), texture_name(texture_name) {}

}  // namespace platformer2d