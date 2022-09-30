#pragma once 

#define flg_status 1

// #ifndef flg_status
//     #define flg_status 0
// #endif

#ifdef flg_status
    #include <string>
    #include <stdio.h>
    #include <fstream>
    #define CUS const unsigned short
    #define CB const bool
    /**
     * @brief flg - PathMarker version 2
     * Stores flags, which can be added in every code stage, if needed. To mark important stuff, exceptions or executed path
     * 
     * @param [version]([type][number])
     * @param version develop version, shows version of software which is currently flagging (most useful in inheritance mechanics)
     * @param type describes important of flag, comparing to others, flags might be "informative", showing "path" or marking "errors"
     * @param number number of flag in this version, shows in numeric order where this flag have his description
     */
    class flg_v2{
    protected:
        struct singleFlag{
            singleFlag* next;
            CUS version;
            const char type;
            CUS number;
            singleFlag* prev;
        };
        singleFlag* pointer;
        // example industrial print data:
        // (000)<(440)>(460)  v(0) t(i) n(0)
        // (440)<(460)>(7b0)  v(0) t(i) n(0)
        // (460)<(7b0)>(7d0)  v(0) t(i) n(1)
        // (7b0)<(7d0)>(7f0)  v(0) t(i) n(2)
        // (7d0)<(7f0)>(810)  v(1) t(i) n(0)
        // (7f0)<(810)>(000)  v(2) t(i) n(1)
    public:
        flg_v2();
        ~flg_v2();

        void add(CUS&, const char&, CUS&);
        void clear();
        void print(const char*, const std::string&, const unsigned long&) const;
        void fileprint(const char*, const std::string&, const unsigned long&) const;
    protected:
        void industrialPrint(const char*) const;
        std::string toHex(CUS&) const;
    };



    flg_v2::flg_v2(){
        this->pointer = NULL;
    }
    flg_v2::~flg_v2(){

    }


    /**
     * adding single flag to flags storage
     * 
     * @param version 
     * @param type 
     * @param number 
     * @return void
     */
    void flg_v2::add(CUS& version, const char& type, CUS& number){
        if(this->pointer == NULL){ // is empty
            singleFlag* handle = new singleFlag{NULL, version, type, number, NULL};
            this->pointer = handle;
            return;
        }
        else{ // already has one flag
            singleFlag* handle = this->pointer;
            while(handle->next != NULL) handle = handle->next;
            handle->next = new singleFlag{NULL, version, type, number, handle};
        }
    }
    /**
     * clears all flags 
     * 
     * @param none
     * @return void
     */
    void flg_v2::clear(){
        if(this->pointer == NULL) return; // is empty
        if(this->pointer->next == NULL){ // has one flag
            delete pointer;
            pointer = NULL;
            return;
        }
        // rest cases
        singleFlag* handle = this->pointer;
        while(handle->next->next != NULL) handle = handle->next; // find last one, it is possible to avoid this line but i am fine with that
        delete handle->next;
        handle->next = NULL;
        this->clear(); // recurse :)
    }
    /**
     * prints flags on command prompt if types and version are acceptable, those whose can not be acceptable will be replaced with - or _
     * 
     * @param description if is not "" prints additional information about actually printed flags status
     * @param types only the mentioned types will be acceptable, if not single one was mentioned, all types will be acceptable
     * @param version only the mentioned versions will be acceptable, if version will be above 0xFFFF all version will be acceptable
     * @return void
     */
    void flg_v2::print(const char* description = "", const std::string& types = "", const unsigned long& version= 0xFFFF+1) const{
        // types - if is "" print all types (info, warning, error, ...)
        // version - if it is above 0xFFFF (unsigned short) print all versions if is below then print only those versions
        if(description != "") printf("print: %s\n", description);
        
        singleFlag* handle = this->pointer;
        unsigned char counter = 0;
        while(handle != NULL){
            // printf("v");
            if((handle->version == version || version > 0xFFFF) && (types.find(handle->type) != std::string::npos || types == "")){
                printf("%s(%c%s) ",this->toHex(handle->version).c_str(), handle->type, this->toHex(handle->number).c_str());
            }
            else{
                if(counter == 9) printf("_ ");
                else printf("- ");
            }
            counter++;
            if(counter == 10) counter = 0;
            handle = handle->next;
        }
        printf("\n");

    }
    /**
     * saves flags to file if types and version are acceptable, those whose can not be acceptable will be replaced with - or _
     * 
     * @param path relative path to file where expected flags status should be saved
     * @param types only the mentioned types will be acceptable, if not single one was mentioned, all types will be acceptable
     * @param version only the mentioned versions will be acceptable, if version will be above 0xFFFF all version will be acceptable
     * @return void
     */
    void flg_v2::fileprint(const char* path, const std::string& types = "", const unsigned long& version= 0xFFFF+1) const{
        // types - if is "" print all types (info, warning, error, ...)
        // version - if it is above 0xFFFF (unsigned short) print all versions if is below then print only those versions
        std::fstream file;
        file.open(path, std::ios::out);
        if(!file.good()){
            printf("can not create file %s\n",path);
            return;
        }
        
        singleFlag* handle = this->pointer;
        unsigned char counter = 0;
        while(handle != NULL){
            if((handle->version == version || version > 0xFFFF) && (types.find(handle->type) != std::string::npos || types == "")){
                file << this->toHex(handle->version).c_str() << "(" << handle->type << this->toHex(handle->number).c_str() << ") ";
            }
            else{
                if(counter == 9) file << "_ ";
                else file << "- ";
            }
            counter++;
            if(counter == 10) counter = 0;
            handle = handle->next;
        }
        file.close();
    }


    /**
     * only 4 print raw data about stored flags, just 4 testing use
     * 
     * @param description if is not "" prints additional information about actually printed flags status
     * @return void
     */
    void flg_v2::industrialPrint(const char* description = "") const{
        if(description != "") printf("print description: %s\n", description);
        if(this->pointer == NULL){
            printf("NULL\n");
            return;
        }
        singleFlag* handle = this->pointer;
        do{
            printf("(%p)< (%p) >(%p)  v(%d) t(%c) n(%d)\n", handle->prev, handle, handle->next, handle->version, handle->type, handle->number);
            handle = handle->next;
        }while(handle != NULL);
    }
    /**
     * change number to 4 digited hex string up to 0xFFFF
     * i know that exists a lot of better ways to do this, but target was to optimize length of hex value depend from the longest one value
     * but 4 now just changing to hex value, satisfies me, when i will have motivation i will finish this
     * 
     * @param number decimal value up to 65535
     * @return max 4 digit hex value as a string
     */
    std::string flg_v2::toHex(CUS& number) const{
        // converting number up to 0xFFFF to hexadecimal
        std::string hex = "";
        #define s(x) ((number/x)%16)
        if(s(0x1000)!=0)hex += (char)(s(0x1000)>9 ? s(0x1000)-10+65 : s(0x1000)+48);
        if(s(0x100)!=0) hex += (char)(s(0x100) >9 ? s(0x100) -10+65 : s(0x100)+48);
        if(s(0x10)!=0)  hex += (char)(s(0x10)  >9 ? s(0x10)  -10+65 : s(0x10)+48);
                        hex += (char)(s(0x1)   >9 ? s(0x1)   -10+65 : s(0x1)+48);
        #undef s
        return hex;
    }

/*
    all defines shortcuts bellow allows to turn off flgv1 extention in order to reduce compute consumption
    and change every flgv1 line to comment
*/

    #if flg_status == 0
        #define flg_start //flg_v2 flgObject;
        #define flg(version, type, number) //flgObject.add(v,t,n);
        #define flg_print1(description) //flgObject.print(description);
        #define flg_print2(description,types) //flgObject.print(description,types);
        #define flg_print3(description,types,version) //flgObject.print(description,types,version);
        #define flg_fileprint1(file) //flgObject.fileprint(file);
        #define flg_fileprint2(file,types) //flgObject.fileprint(file,types);
        #define flg_fileprint3(file,types,version) //flgObject.fileprint(file,types,version);
        #define flg_clear //flgObject.clear();
        // #define flg_inprint(description) //flgObject.industrialPrint(description);
    #elif flg_status == 1
        #define flg_start flg_v2 flgObject;
        #define flg(version, type, number) flgObject.add(version,type,number);
        #define flg_print1(description) flgObject.print(description);
        #define flg_print2(description,types) flgObject.print(description,types);
        #define flg_print3(description,types,version) flgObject.print(description,types,version);
        #define flg_fileprint1(file) flgObject.fileprint(file);
        #define flg_fileprint2(file,types) flgObject.fileprint(file,types);
        #define flg_fileprint3(file,types,version) flgObject.fileprint(file,types,version);
        #define flg_clear flgObject.clear();
        // #define flg_inprint(description) flgObject.industrialPrint(description);
    #elif flg_status == 2
        #define flg_start(object) //flg_v2 object;
        #define flg(object,version,type,number) //object.add(version,type,number);
    #elif flg_status == 3
        #define flg_start(object) flg_v2 object;
        #define flg(object,version,type,number) object.add(version,type,number);
    #endif

    #undef flg_status
#endif


