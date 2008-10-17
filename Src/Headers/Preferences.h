#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <app/Message.h>
#include <app/Messenger.h>
#include <support/String.h>
#include <support/Archivable.h>
#include <support/Flattenable.h>
#include <storage/Entry.h>
#include "Exception.h"

class Preferences	:	public BArchivable
{
	public:
		Preferences(BString prefsName);
		Preferences(BMessage* archive);
		virtual					~Preferences();
		
		virtual BString			GetName();
		virtual void			SetName(BString prefsName);
			
		virtual status_t 		Archive(BMessage* archive, bool deep = true) const;
		static BArchivable* 	Instantiate(BMessage* archive);
	
	public:		
		//message add methods
		void	 				AddData(const char* name, type_code type, const void* data, 
									ssize_t numBytes, bool fixedSize = true,  int32 numItems = 1) throw(Exception);
        void	 				AddBool(const char* name, bool aBool) throw(Exception);
        void	 				AddInt8(const char* name, int8 anInt8) throw(Exception);
        void	 				AddInt16(const char* name, int16 anInt16) throw(Exception);
        void	 				AddInt32(const char* name, int32 anInt32) throw(Exception);
        void	 				AddInt64(const char* name, int64 anInt64) throw(Exception);
        void	 				AddFloat(const char* name, float aFloat)  throw(Exception);
		void		 			AddDouble(const char* name, double aDouble) throw(Exception);
		void	 				AddString(const char* name, const char* string) throw(Exception); 
		void	 				AddString(const char* name, const BString string) throw(Exception);
		void	 				AddPoint(const char* name, BPoint point) throw(Exception);
		void	 				AddRect(const char* name, BRect rect) throw(Exception);
		void	 				AddRef(const char* name, const entry_ref* ref) throw(Exception);
		void					AddMessage(const char* name, const BMessage* message) throw(Exception);
		void	 				AddMessenger(const char* name, BMessenger messenger) throw(Exception);
		void	 				AddPointer(const char* name, const void* pointer) throw(Exception);
		void	 				AddFlat(const char* name, BFlattenable* object, int32 numItems = 1) throw(Exception);	
		
		int32 					CountNames(type_code type) const;
		
		//message find methods
		void 					FindData(const char* name, type_code type, int32 index, const void** data,
                  							ssize_t* numBytes) const throw(Exception);         
        void 					FindData(const char* name, type_code type, const void** data,
                  							ssize_t* numBytes) const throw(Exception);                  
		bool 					FindBool(const char* name, int32 index) const throw(Exception);         
        bool 					FindBool(const char* name) const throw(Exception);        
		int8 					FindInt8(const char* name, int32 index) const throw(Exception); 
		int8 					FindInt8(const char* name) const throw(Exception);		
		int16 					FindInt16(const char* name,int32 index) const throw(Exception); 
		int16 					FindInt16(const char* name) const throw(Exception);
		int32 					FindInt32(const char* name, int32 index) const throw(Exception); 
		int32 					FindInt32(const char* name) const throw(Exception);
		int64 					FindInt64(const char* name, int32 index) const throw(Exception); 
		int64 					FindInt64(const char* name) const throw(Exception);
		float 					FindFloat(const char* name, int32 index) const throw(Exception); 
		float 					FindFloat(const char* name) const throw(Exception);
		double 					FindDouble(const char* name, int32 index) const throw(Exception); 
		double 					FindDouble(const char* name) const throw(Exception);
		BString 				FindString(const char* name, int32 index) const throw(Exception); 
		BString 				FindString(const char* name) const throw(Exception); 
		BPoint 					FindPoint(const char* name, int32 index) const throw(Exception); 
		BPoint 					FindPoint(const char* name) const throw(Exception);
		BRect 					FindRect(const char* name, int32 index) const throw(Exception); 
		BRect 					FindRect(const char* name) const throw(Exception);
		entry_ref 				FindRef(const char* name, int32 index) const throw(Exception); 
		entry_ref 				FindRef(const char* name) const throw(Exception);
		BMessage				FindMessage(const char* name, int32 index) const throw(Exception); 
		BMessage				FindMessage(const char* name) const throw(Exception);
		BMessenger 				FindMessenger(const char* name, int32 index) const throw(Exception); 
		BMessenger				FindMessenger(const char* name) const throw(Exception);
		void* 					FindPointer(const char* name, int32 index) const throw(Exception); 
		void* 					FindPointer(const char* name) const throw(Exception);
	/*	BFlattenable			FindFlat(const char* name, int32 index) const; 
		BFlattenable			FindFlat(const char* name) const;*/
			
		void					GetInfo(const char* name, type_code* typeFound, int32* countFound = NULL) const throw(Exception); 
		void					GetInfo(const char* name, type_code* typeFound, bool* fixedSize) const throw(Exception); 
        void					GetInfo(type_code type, int32 index, char** nameFound, type_code* typeFound, int32* countFound = NULL) const throw(Exception); 
        void					MakeEmpty() throw(Exception);
        bool					IsEmpty() const;
        void 					PrintToStream() const;
        void					RemoveName(const char* name) throw(Exception);
        void					RemoveData(const char* name, int32 index = 0) throw(Exception);
        
        //message replace methods
        
	private:
		void					HandleAddError(status_t error) throw(Exception);
		void					HandleFindError(status_t error) const throw(Exception);
				
	private:
		BString				m_prefsName;
		BMessage			m_prefsMessage;
};

#endif
