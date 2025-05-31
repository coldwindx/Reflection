#include "raw_type_descriptor_builder.h"
#include "registry.h"
namespace reflect
{
    RawTypeDescriptorBuilder::RawTypeDescriptorBuilder(const string &name) : desc(std::make_unique<TypeDescriptor>())
    {
        desc->name = name;
    }

    RawTypeDescriptorBuilder::~RawTypeDescriptorBuilder()
    {
        Registry::instance().push(std::move(desc));
    }
}