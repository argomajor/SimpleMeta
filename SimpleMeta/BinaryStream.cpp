#include "BinaryStream.hpp"

#include "StaticTypeId.hpp"
#include "MetaSerialization.hpp"


BinarySaver::BinarySaver(std::ostream& stream) : mStream(stream.rdbuf())
{
  mDirection = SerializerDirection::Saving;
}

bool BinarySaver::SerializePrimitive(const BoundType& boundType, char* data)
{
  Write(data, boundType.mSizeInBytes);
  return true;
}

bool BinarySaver::SerializePrimitive(const BoundType& boundType, std::string& data)
{
  Write(data.size());
  mStream.write(data.data(), data.size());
  return true;
}

bool BinarySaver::BeginStringTable(int& count) {
    mStream.write((char*)&count, sizeof(count));
    return true;
}

bool BinarySaver::EndStringTable() {
    return true;
}

bool BinarySaver::BeginStringTableEntry(std::string& name) {
    Write(name.size());
    mStream.write(name.data(), name.size());
    return true;
}

bool BinarySaver::EndStringTableEntry() {
    return true;
}


bool BinarySaver::BeginObject(PolymorphicInfo& info)
{
  Write(info.mId.mId);
  return true;
}

bool BinarySaver::BeginArray(size_t& count)
{
  Write(count);
  return true;
}

bool BinarySaver::Write(char* data, size_t sizeInBytes)
{
  mStream.write(data, sizeInBytes);
  return true;
}

bool BinarySaver::Write(size_t data, size_t sizeInBytes)
{
  Write((char*)&data, sizeInBytes);
  return true;
}

bool BinarySaver::Write(size_t data)
{
  Write(data, sizeof(data));
  return true;
}

BinaryLoader::BinaryLoader(std::istream& stream) : mStream(stream.rdbuf())
{
  mDirection = SerializerDirection::Loading;
}

bool BinaryLoader::SerializePrimitive(const BoundType& boundType, char* data)
{
  mStream.read(data, boundType.mSizeInBytes);
  return true;
}

bool BinaryLoader::SerializePrimitive(const BoundType& boundType, std::string& data)
{
  new(&data) std::string();
  size_t sizeInBytes = 0;
  Read(sizeInBytes);
  data.resize(sizeInBytes);
  mStream.read((char*)data.data(), sizeInBytes);
  return false;
}

bool BinaryLoader::BeginStringTable(int& count) {
    char* data = (char*)&count;
    mStream.read(data, sizeof(count));
    return true;
}

bool BinaryLoader::EndStringTable() {
    return true;
}

bool BinaryLoader::BeginStringTableEntry(std::string& name) {
    size_t sizeInBytes = 0;
    Read(sizeInBytes);
    name.resize(sizeInBytes);
    mStream.read((char*)name.data(), sizeInBytes);
    return true;
}

bool BinaryLoader::EndStringTableEntry() {
    return true;
}

bool BinaryLoader::ForAllMembers(int size, std::function<void(Serializer&, const std::string&)> callback) {
    for (int i = 0; i < size; ++i) {
        std::string memberName("");
        callback(*this, memberName);
    }
    return true;
}

bool BinaryLoader::BeginObject()
{
  return false;
}

bool BinaryLoader::BeginObject(PolymorphicInfo& info)
{
  return Read(info.mId.mId);
}

bool BinaryLoader::BeginArray(size_t& count)
{
  Read(count);
  return true;
}

bool BinaryLoader::EndArray()
{
  return true;
}

bool BinaryLoader::EndObject()
{
  return true;
}

bool BinaryLoader::SerializeObject(BoundType& boundType, char* data)
{
  return SerializeProperties(boundType, data);
}

bool BinaryLoader::Read(char* data, size_t sizeInBytes)
{
  mStream.read(data, sizeInBytes);
  return true;
}

bool BinaryLoader::Read(size_t& data, size_t sizeInBytes)
{
  data = 0;
  Read((char*)&data, sizeInBytes);
  return true;
}

bool BinaryLoader::Read(size_t& data)
{
  Read(data, sizeof(data));
  return true;
}
