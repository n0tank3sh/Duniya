#pragma once
#include <iostream>
#include <Exception.hpp>

#define CHECKIS if(is == nullptr) throw CException(__LINE__, __FILE__, "SerilzerSystem", "is is null")
#define CHECKOS if(os == nullptr) throw CException(__LINE__, __FILE__, "SerilzerSystem", "os is null")

class SerializerSystem
{
private:
	SerializerSystem();
public:
	static SerializerSystem* init();
	template<class T>
	void Serialize(const T& var);
	template<class T>
	void Deserialize(T& var);
	void SetIStream(std::istream& is);
	void SetOStream(std::ostream& os);
	// Using iostream pointer insteal of the reference for not delaring implicitly 
public:
	std::istream* is;
	std::ostream* os;
	static SerializerSystem* singleton;

};
