#include "SerializerSystem.h"
#include "ecs.h"
#include "GraphicsComponent.h"
#include "commoncomponent.h"

SerializerSystem* SerializerSystem::singleton = nullptr;

SerializerSystem::SerializerSystem()
{
	is = nullptr;
	os = nullptr;
}

void SerializerSystem::SetIStream(std::istream& is)
{
	this->is = &is;
}

void SerializerSystem::SetOStream(std::ostream& os)
{
	this->os = &os;
}

SerializerSystem* SerializerSystem::init()
{
	if(singleton == nullptr)
	singleton = new SerializerSystem(); 
	return singleton;
}

template<class T>
void SerializerSystem::Serialize(const T& var)
{
	CHECKOS;
	os->write((char*)&var, sizeof(T));
}

template<class T>
void SerializerSystem::Deserialize(T& var)
{
	CHECKIS;
	is->read((char*)&var, sizeof(T));
}

template<>
void SerializerSystem::Serialize<Mesh>(const Mesh& var)
{
	CHECKOS;
	assert(var.verticies != nullptr);
	assert(var.indicies  != nullptr);
	uint32_t meshSizes[2] = { (uint32_t)var.vertexCount, (uint32_t)var.indexCount};	
	os->write((char*)meshSizes, sizeof(uint32_t) * 2);
	os->write((char*)var.verticies, sizeof(Vertex) * var.vertexCount);
	os->write((char*)var.indicies, sizeof(uint32_t) * var.indexCount);
}

template<>
void SerializerSystem::Deserialize<Mesh>(Mesh& var)
{
	CHECKIS;

	uint32_t meshSizes[2];
	is->read((char*)meshSizes, sizeof(uint32_t) * 2);
	var.vertexCount = meshSizes[0];
	var.indexCount = meshSizes[1];
	var.verticies = new Vertex[var.vertexCount];
	var.indicies = new uint32_t[var.indexCount];
	is->read((char*)var.verticies, sizeof(Vertex) * var.vertexCount);
	is->read((char*)var.indicies, sizeof(uint32_t) * var.indexCount);
	for(uint32_t i = 0; i < var.indexCount; i++)
	{
		std::cout << var.indicies[i] << " ";
	}
}


template<>
void SerializerSystem::Serialize<Texture>(const Texture& var)
{
	CHECKOS;
	os->write((char*)&var.width, sizeof(uint32_t));
	os->write((char*)&var.height, sizeof(uint32_t));
	os->write((char*)&var.depth, sizeof(uint32_t));
	os->write((char*)&var.type, sizeof(uint32_t));
	os->write((char*)&var.sizet, sizeof(uint32_t));
	os->write((char*)var.data, var.sizet);
}

template<>
void SerializerSystem::Deserialize<Texture>(Texture& var)
{
	CHECKIS;
	is->read((char*)&var.width, sizeof(uint32_t));
	is->read((char*)&var.height, sizeof(uint32_t));
	is->read((char*)&var.depth, sizeof(uint32_t));
	is->read((char*)&var.type, sizeof(uint32_t));
	is->read((char*)&var.sizet, sizeof(uint32_t));
	is->read((char*)var.data, var.sizet);

}

template<>
void SerializerSystem::Serialize<Scene::IComponentArray>(const Scene::IComponentArray& var)
{
	auto comptritr = var.components.begin();
	uint32_t totalSize = var.components.size();
	os->write((char*)&totalSize, sizeof(uint32_t));
	while(comptritr != var.components.end())
	{
		os->write((char*)&comptritr->first, sizeof(uint32_t));
		if(comptritr->first == ComponentType::TRANSFORM)
		{
			Transform* transform = (Transform*)comptritr->second.base->GetPointer();
			SerializerSystem::singleton->Serialize<Transform>(*transform);
		}
		else if(comptritr->first == ComponentType::MESH)
		{
			Mesh* mesh = (Mesh*)comptritr->second.base->GetPointer();
			SerializerSystem::singleton->Serialize<Mesh>(*mesh);
		}
		else if(comptritr->first == ComponentType::TEXTURE)
		{
			Texture* texture = (Texture*)comptritr->second.base->GetPointer();
			SerializerSystem::singleton->Serialize<Texture>(*texture);
		}
	comptritr++;
	}
}

template<>
void SerializerSystem::Deserialize<Scene::IComponentArray>(Scene::IComponentArray& var)
{
	uint32_t totalSize;
	is->read((char*)&totalSize, sizeof(uint32_t));
	//std::cout << "Totatl Size: " << totalSize << std::endl;
	//in.read((char*)componentArray->componentTypes.data(), sizeof(uint32_t) * totalSize);
	for(uint32_t itr = 0; itr < totalSize; itr++)
	{
		std::cout << "Is it even entering here " << std::endl;
		ComponentType componentTypeTemp;
		uint32_t componentTypeinInt;
		is->read((char*)&componentTypeinInt, sizeof(uint32_t));
		componentTypeTemp = static_cast<ComponentType>(componentTypeinInt);
		std::cout << "The size of the component array: " <<  var.components.size() << std::endl;
		ComponentPtr& componentPtr= var.components[componentTypeTemp];
		if(componentTypeTemp == ComponentType::MESH)
		{
			componentPtr.base = new ComponentPtr::Impl<Mesh>();
			componentPtr.base->Create();
			Mesh *mesh = (Mesh*)componentPtr.base->GetPointer();
			SerializerSystem::singleton->Deserialize<Mesh>(*mesh);
		}
		else if(componentTypeTemp == ComponentType::TEXTURE)
		{				
			componentPtr.base = new ComponentPtr::Impl<Texture>();
			componentPtr.base->Create();
			Texture* texture = (Texture*)componentPtr.base->GetPointer();
			SerializerSystem::singleton->Deserialize<Texture>(*texture);
		}
		else if(componentTypeTemp == ComponentType::TRANSFORM)
		{
			componentPtr.base = new ComponentPtr::Impl<Transform>();
			componentPtr.base->Create();
			Transform* transform = (Transform*)componentPtr.base->GetPointer();
			SerializerSystem::singleton->Deserialize<Transform>(*transform);
		}
	}
}

template<>
void SerializerSystem::Serialize<ComponentTypeMap>(const ComponentTypeMap& var)
{
	uint32_t typeMapSize = var.size();
	os->write((char*)&typeMapSize, sizeof(uint32_t));
	for(auto& i: var)
	{
		std::string temp;
		if(i.first == std::type_index(typeid(Transform)))
			temp = "Transform";
		else if(i.first == std::type_index(typeid(Mesh)))
			temp = "Mesh";
		else if(i.first == std::type_index(typeid(Texture)))
			temp = "Texture";
		else if(i.first == std::type_index(typeid(Material)))
			temp = "Material";
		uint32_t tempSize = temp.size();
		std::cout << "The Temp Size: " << tempSize << std::endl;
		//std::cout << "Temp Name " <<  temp << std::endl;
		os->write((char*)&tempSize, sizeof(uint32_t));
		os->write(temp.data(), tempSize);
		os->write((char*)&i.second, sizeof(uint32_t));
	}
}

template<>
void SerializerSystem::Deserialize<ComponentTypeMap>(ComponentTypeMap& var)
{
	uint32_t typeMapSize;
	SerializerSystem::Deserialize<uint32_t>(typeMapSize);
	
	while(typeMapSize--)
	{
		uint32_t tempSize;
		uint32_t componentTypeUI;
		char* typeNameChar;
		SerializerSystem::Deserialize<uint32_t>(tempSize);
		typeNameChar = new char[tempSize];
		is->read(typeNameChar, tempSize);
		SerializerSystem::Deserialize<uint32_t>(componentTypeUI);

		std::type_index* typeIndex = nullptr;
		std::string typeName(typeNameChar, tempSize);
		std::cout << "String size " << tempSize << std::endl;
		std::cout <<"The type name is: " <<  typeName << std::endl;
		
		if(typeName == "Transform")
			typeIndex = new std::type_index(typeid(Transform));
		else if(typeName == "Mesh")
			typeIndex = new std::type_index(typeid(Mesh));
		else if(typeName == "Texture")
			typeIndex = new std::type_index(typeid(Texture));
		else if(typeName == "Material")
			typeIndex = new std::type_index(typeid(Material));
		if(var.find(*typeIndex) == var.end())
		var.insert(std::make_pair(*typeIndex, static_cast<ComponentType>(componentTypeUI)));
		delete[] typeNameChar;
		typeNameChar = nullptr;
	}
}

template<>
void SerializerSystem::Serialize<Scene::Entities>(const Scene::Entities& var)
{
	uint32_t entitySize = var.size();
	singleton->Serialize<uint32_t>(entitySize);
	for(auto& i: var)
	{
		singleton->Serialize<uint32_t>(i.first);
		singleton->Serialize<Scene::IComponentArray>(*(i.second.get()));
	}
}

template<>
void SerializerSystem::Deserialize<Scene::Entities>(Scene::Entities& var)
{
	uint32_t entitySize;
	singleton->Deserialize<uint32_t>(entitySize);
	while(entitySize--)
	{
		uint32_t entity;
		singleton->Deserialize<uint32_t>(entity);
		var.insert(std::make_pair(entity, std::unique_ptr<Scene::IComponentArray>(new Scene::IComponentArray)));
		singleton->Deserialize<Scene::IComponentArray>(*(var[entity].get()));
	}
}
