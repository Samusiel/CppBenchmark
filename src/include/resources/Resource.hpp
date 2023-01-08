#pragma once

namespace Resources {

template <typename Resource>
class ResourceFactory;

class IResource {
public:
    virtual ~IResource() = 0;
};

template <typename Resource>
class ResourceBase {
public:
    using ResourceFactory = ResourceFactory<Resource>;

public:
};

} // namespace Resources