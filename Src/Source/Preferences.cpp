#ifndef PREFERENCES_H
#include "Preferences.h"
#endif

#include <iostream>

Preferences::Preferences(BString prefsName)
				:	BArchivable(),
					m_prefsName(prefsName)
{	
}

Preferences::Preferences(BMessage* archive)
				:	BArchivable(archive)
{
	//create a preferences object out of an archived message
	archive->FindString("Preferences::prefsName",&m_prefsName);
	archive->FindMessage("Preferences::prefsMessage", &m_prefsMessage);
}

Preferences::~Preferences()
{	
}

BString Preferences::GetName()
{
	return m_prefsName;
}

void Preferences::SetName(BString prefsName)
{
	m_prefsName = prefsName;
}

status_t Preferences::Archive(BMessage* archive, bool deep) const
{
	archive->AddString("class", "Preferences");
	archive->AddString("Preferences::prefsName",m_prefsName);
	archive->AddMessage("Preferences::prefsMessage", &m_prefsMessage);
	return B_OK;
}

BArchivable* Preferences::Instantiate(BMessage* archive)
{
   if (!validate_instantiation(archive, "Preferences"))
      return NULL;
   return new Preferences(archive);
}

void Preferences::AddData(const char* name, type_code type, const void* data, 
					ssize_t numBytes, bool fixedSize = true,  int32 numItems = 1) throw(Exception)
{
	status_t error = m_prefsMessage.AddData(name, type, data, numBytes, fixedSize, numItems);
	HandleAddError(error);
}

void Preferences::AddBool(const char* name, bool aBool) throw(Exception)
{
	status_t error = m_prefsMessage.AddBool(name, aBool);
	HandleAddError(error);
}

void Preferences::AddInt8(const char* name, int8 anInt8) throw(Exception)
{
	status_t error = m_prefsMessage.AddInt8(name, anInt8);
	HandleAddError(error);
}

void Preferences::AddInt16(const char* name, int16 anInt16) throw(Exception)
{
	status_t error = m_prefsMessage.AddInt16(name, anInt16);
	HandleAddError(error);
}

void Preferences::AddInt32(const char* name, int32 anInt32) throw(Exception)
{
	status_t error = m_prefsMessage.AddInt32(name, anInt32);
	HandleAddError(error);
}

void Preferences::AddInt64(const char* name, int64 anInt64) throw(Exception)
{
	status_t error = m_prefsMessage.AddInt64(name, anInt64);
	HandleAddError(error);
}

void Preferences::AddFloat(const char* name, float aFloat) throw(Exception)
{
	status_t error = m_prefsMessage.AddFloat(name, aFloat);
	HandleAddError(error);
}

void Preferences::AddDouble(const char* name, double aDouble) throw(Exception)
{
	status_t error = m_prefsMessage.AddDouble(name, aDouble);
	HandleAddError(error);
}

void Preferences::AddString(const char* name, const char* string) throw(Exception)
{
	status_t error = m_prefsMessage.AddString(name, string);
	HandleAddError(error);
}

void Preferences::AddString(const char* name, const BString string) throw(Exception)
{
	status_t error = m_prefsMessage.AddString(name, string);
	HandleAddError(error);
}

void Preferences::AddPoint(const char* name, BPoint point) throw(Exception)
{
	status_t error = m_prefsMessage.AddPoint(name, point);
	HandleAddError(error);
}

void Preferences::AddRect(const char* name, BRect rect) throw(Exception)
{
	status_t error = m_prefsMessage.AddRect(name, rect);
	HandleAddError(error);
}

void Preferences::AddRef(const char* name, const entry_ref* ref) throw(Exception)
{
	status_t error = m_prefsMessage.AddRef(name, ref);
	HandleAddError(error);
}

void Preferences::AddMessage(const char* name, const BMessage* message) throw(Exception)
{
	status_t error = m_prefsMessage.AddMessage(name, message);
	HandleAddError(error);
}

void Preferences::AddMessenger(const char* name, BMessenger messenger) throw(Exception)
{
	status_t error = m_prefsMessage.AddMessenger(name, messenger);
	HandleAddError(error);
}

void Preferences::AddPointer(const char* name, const void* pointer) throw(Exception)
{
	status_t error = m_prefsMessage.AddPointer(name, pointer);
	HandleAddError(error);
}

void Preferences::AddFlat(const char* name, BFlattenable* object, int32 numItems = 1) throw(Exception)
{
	status_t error = m_prefsMessage.AddFlat(name, object, numItems);
	HandleAddError(error);
}

int32 Preferences::CountNames(type_code type) const 
{
	return m_prefsMessage.CountNames(type);
}

//message find methods
void Preferences::FindData(const char* name, type_code type, int32 index, const void** data,
                							ssize_t* numBytes) const throw(Exception)
{
	status_t error = m_prefsMessage.FindData(name, type, index, data, numBytes);
	HandleFindError(error);	
}         

void Preferences::FindData(const char* name, type_code type, const void** data,
                							ssize_t* numBytes) const throw(Exception)
{
	status_t error = m_prefsMessage.FindData(name, type, data, numBytes);
	HandleFindError(error);
}
                  
bool Preferences::FindBool(const char* name, int32 index) const throw(Exception)
{
	bool aBool;
	status_t error = m_prefsMessage.FindBool(name, index, &aBool); 
	HandleFindError(error);
	return aBool;
}
        
bool Preferences::FindBool(const char* name) const throw(Exception)
{
	bool aBool;
	status_t err = m_prefsMessage.FindBool(name, &aBool);
	HandleFindError(err);
	return aBool;
}
          
int8 Preferences::FindInt8(const char* name, int32 index) const throw(Exception)
{
	int8 anInt;
	status_t error = m_prefsMessage.FindInt8(name, index, &anInt);
	HandleFindError(error);
	return anInt;
}
  
int8 Preferences::FindInt8(const char* name) const throw(Exception)
{
	int8 anInt;
	status_t error = m_prefsMessage.FindInt8(name, &anInt);
	HandleFindError(error);
	return anInt;
}
 	
int16 Preferences::FindInt16(const char* name,int32 index) const throw(Exception)
{
	int16 anInt;
	status_t error = m_prefsMessage.FindInt16(name, index, &anInt);
	HandleFindError(error);
	return anInt;
}
 
int16 Preferences::FindInt16(const char* name) const throw(Exception)
{
	int16 anInt;
	status_t error = m_prefsMessage.FindInt16(name, &anInt);
	HandleFindError(error);
	return anInt;
}
 
int32 Preferences::FindInt32(const char* name, int32 index) const throw(Exception)
{
	int32 anInt;
	status_t error = m_prefsMessage.FindInt32(name, index, &anInt);
	HandleFindError(error);
	return anInt;
}
  
int32 Preferences::FindInt32(const char* name) const throw(Exception)
{
	int32 anInt;
	status_t error = m_prefsMessage.FindInt32(name, &anInt);
	HandleFindError(error);
	return anInt;
}
 
int64 Preferences::FindInt64(const char* name, int32 index) const throw(Exception)
{
	int64 anInt;
	status_t error = m_prefsMessage.FindInt64(name, index, &anInt);
	HandleFindError(error);
	return anInt;
}
 
int64 Preferences::FindInt64(const char* name) const throw(Exception)
{
	int64 anInt;
	status_t error = m_prefsMessage.FindInt64(name, &anInt);
cout << "error = " << error << endl;		
	HandleFindError(error);
cout << "after handlefinderror" << endl;	
	return anInt;
}
 
float Preferences::FindFloat(const char* name, int32 index) const throw(Exception)
{
	float aFloat;
	status_t error = m_prefsMessage.FindFloat(name, index, &aFloat);
	HandleFindError(error);
	return aFloat;
}
 
float Preferences::FindFloat(const char* name) const throw(Exception)
{
	float aFloat;
	status_t error = m_prefsMessage.FindFloat(name, &aFloat);
	HandleFindError(error);
	return aFloat;
}
 
double Preferences::FindDouble(const char* name, int32 index) const throw(Exception)
{
	double aDouble;
	status_t error = m_prefsMessage.FindDouble(name, index, &aDouble);
	HandleFindError(error);
	return aDouble;
}
 
double Preferences::FindDouble(const char* name) const throw(Exception)
{
	double aDouble;
	status_t error = m_prefsMessage.FindDouble(name, &aDouble);
	HandleFindError(error);
	return aDouble;
}
 
BString Preferences::FindString(const char* name, int32 index) const throw(Exception)
{
	BString aString;
	status_t error = m_prefsMessage.FindString(name, index, &aString);
	HandleFindError(error);
	return aString;
}
  
BString Preferences::FindString(const char* name) const throw(Exception) 
{
	BString aString;
	status_t error = m_prefsMessage.FindString(name, &aString);
	HandleFindError(error);
	return aString;
}
 
BPoint Preferences::FindPoint(const char* name, int32 index) const throw(Exception)
{
	BPoint aPoint;
	status_t error = m_prefsMessage.FindPoint(name, index, &aPoint);
	HandleFindError(error);
	return aPoint;
}
  
BPoint Preferences::FindPoint(const char* name) const throw(Exception)
{
	BPoint aPoint;
	status_t error = m_prefsMessage.FindPoint(name, &aPoint);
	HandleFindError(error);
	return aPoint;
}
  
BRect Preferences::FindRect(const char* name, int32 index) const  throw(Exception)
{
	BRect rect;
	status_t error = m_prefsMessage.FindRect(name, index, &rect);
	HandleFindError(error);
	return rect;
}
 
BRect Preferences::FindRect(const char* name) const throw(Exception)
{
	BRect rect;
	status_t error = m_prefsMessage.FindRect(name, &rect);
	HandleFindError(error);
	return rect;
}
 
entry_ref Preferences::FindRef(const char* name, int32 index) const throw(Exception)
{
	entry_ref ref;
	status_t error = m_prefsMessage.FindRef(name, index, &ref);
	HandleFindError(error);
	return ref;
}
  
entry_ref Preferences::FindRef(const char* name) const throw(Exception)
{
	entry_ref ref;
	status_t error = m_prefsMessage.FindRef(name, &ref);
	HandleFindError(error);
	return ref;
}
 
BMessage Preferences::FindMessage(const char* name, int32 index) const throw(Exception)
{
	BMessage message;
	status_t error = m_prefsMessage.FindMessage(name, index, &message);
	HandleFindError(error);
	return message;
}
 
BMessage Preferences::FindMessage(const char* name) const throw(Exception)
{
	BMessage message;
	status_t error = m_prefsMessage.FindMessage(name, &message);
	HandleFindError(error);
	return message;
}
 
BMessenger Preferences::FindMessenger(const char* name, int32 index) const throw(Exception)
{
	BMessenger messenger;
	status_t error = m_prefsMessage.FindMessenger(name, index, &messenger);
	HandleFindError(error);
	return messenger;
}
   
BMessenger Preferences::FindMessenger(const char* name) const throw(Exception)
{
	BMessenger messenger;
	status_t error = m_prefsMessage.FindMessenger(name, &messenger);
	HandleFindError(error);
	return messenger;
}
 
void* Preferences::FindPointer(const char* name, int32 index) const throw(Exception)
{
	void *pointer;
	status_t error = m_prefsMessage.FindPointer(name, index, &pointer);
	HandleFindError(error);
	return pointer;
}
 
void* Preferences::FindPointer(const char* name) const throw(Exception)
{
	void *pointer;
	status_t error = m_prefsMessage.FindPointer(name, &pointer);
	HandleFindError(error);
	return pointer;
}
 
/*BFlattenable Preferences::FindFlat(const char* name, int32 index) const
{
	BFlattenable flat;
	status_t error = m_prefsMessage->FindFlat(name, index, &flat);
	HandleFindError(error);
	return flat;
}
 
BFlattenable Preferences::FindFlat(const char* name) const 
{
	BFlattenable flat;
	status_t error = m_prefsMessage->FindFlat(name, &flat);
	HandleFindError(error);
	return flat;
}*/

void Preferences::GetInfo(const char* name, type_code* typeFound, int32* countFound) const  throw(Exception)
{
	status_t error = m_prefsMessage.GetInfo(name, typeFound, countFound);
	HandleFindError(error);
}

void Preferences::GetInfo(const char* name, type_code* typeFound, bool* fixedSize) const  throw(Exception)
{
	status_t error = m_prefsMessage.GetInfo(name, typeFound, fixedSize);
	HandleFindError(error);
}

void Preferences::GetInfo(type_code type, int32 index, char** nameFound, type_code* typeFound, int32* countFound) const throw(Exception) 
{
	status_t error = m_prefsMessage.GetInfo(type, index, nameFound, typeFound, countFound);
	HandleFindError(error);
}

void Preferences::MakeEmpty() throw(Exception)
{
	status_t error = m_prefsMessage.MakeEmpty();
	if (error == B_ERROR)
	{
		throw Exception("MakeEmpty failed, the meessage cannot be altered.");
	}
}

bool Preferences::IsEmpty() const
{
	return m_prefsMessage.IsEmpty();
}

void Preferences::PrintToStream() const
{
	m_prefsMessage.PrintToStream();
}

void Preferences::RemoveName(const char* name) throw(Exception)
{
	status_t error = m_prefsMessage.RemoveName(name);
	HandleFindError(error);
}

void Preferences::RemoveData(const char* name, int32 index) throw(Exception)
{
	status_t error = m_prefsMessage.RemoveData(name, index);
	HandleFindError(error);
}
        		
void Preferences::HandleAddError(status_t error) throw(Exception)
{
	//convert an error that occurred while adding something into an exception
	if (error == B_ERROR)
	{
		throw Exception("Error: the data is too massive to be added to the message.");
	}
	else if (error == B_BAD_TYPE)
	{
		throw Exception("Bad type: the data can't be added to the existing array because it's the wrong type");
	}
	else if (error == B_NO_MEMORY)
	{
		throw Exception("No memory: the BMessage can't get enough memory to hold the data");
	}
	else if (error == B_BAD_VALUE)
	{
		throw Exception("Bad Value: the proposed name for the data is longer than 255 bytes.");
	}  
}

void Preferences::HandleFindError(status_t error) const throw(Exception)
{
	//convert an error that occurred when trying to find something into an exception
	if (error == B_ERROR)
	{
		throw Exception("Unknown Error.");
	}
	if (error == B_NAME_NOT_FOUND)
	{
		throw Exception("Name not found: can't find any data associated with the name given.");
	}
	else if (error == B_BAD_TYPE)
	{
		throw Exception("Bad type: can't find name data of the requested type.");
	}
	else if (error == B_BAD_INDEX)
	{
		throw Exception("Bad index: the given index is out of range.");
	}
	else if (error == B_BAD_VALUE)
	{
		throw Exception("Bad value:the value passed was a negative number.");
	}
}
 