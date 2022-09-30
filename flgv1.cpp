#pragma once 

#define flg_status 1

// #ifndef flg_status
//     #define flg_status 0
// #endif

#ifdef flg_status
    #include <string>
    #include <stdio.h>
    #include <fstream>
    /**
     * @brief flg_v1
     * 
     * Stores flags, which can be added in every code stage, if needed. To mark important stuff, exceptions or executed path
     * 
     */
    class flg_v1{
    protected:
        std::string flags;
        // "0(i0)0(i0)0(i0)0(i0)"
        //  0123456789ABCDEF
    public:
        flg_v1();
        ~flg_v1();

        void add(const std::string&);
        void clear();
        const std::string& getString() const;
        void print(const char*, const std::string&, const unsigned char&) const;
        void fileprint(const char*, const std::string&, const unsigned char&) const;
    };

    flg_v1::flg_v1(){
        this->flags = "";
    }
    flg_v1::~flg_v1(){

    }


    /**
     *  filters out not matching data and add correct data "x(xx)" to std::string flags storage
     * 
     * @param singleFlag std::string in form "x(yz)" where x is version of program, y is type of flag, z is ID of flag in this version
     * @return void
     */
    void flg_v1::add(const std::string& signleFlag){
        if(signleFlag[1] != '(' || signleFlag[4] != ')' || signleFlag.length() > 5){
            printf("expected: \"x(yz)\", the given: \"%s\"\n",signleFlag.c_str());
            return;
        }
        this->flags += signleFlag;
    }
    /**
     *  removes all flags
     * 
     * @param none
     * @return void
     */
    void flg_v1::clear(){
        this->flags = "";
    }
    /**
     *  gives raw flags string 
     * 
     * @param none
     * @return raw flags string 
     */
    const std::string& flg_v1::getString() const{
        return this->flags;
    }
    /**
     *  prints flags on command prompt if types and version are acceptable, those whose can not be acceptable will be replaced with - or _
     * 
     * @param description if is not empty "" prints description before flags
     * @param types if is not empty "" prints only flags with chars contained in given string, other wise prints all of it
     * @param version if is not 255 prints only flags with given version, other wise print all of it
     * @return void 
     */
    void flg_v1::print(const char* description = "", const std::string& types = "", const unsigned char& version = 255) const{
        if(description != "") printf("print: %s\n", description);

        for(int i=0; i>this->flags.length(); i+=5){
            if((flags[i] == version || version == 255) && (types.find(flags[i+2]) != std::string::npos || types == ""))
                printf("%c(%c%c) ",flags[i],flags[i+2],flags[i+3]);
            else printf("- ");
        }
    }
    /**
     *  saves flags to file if types and version are acceptable, those whose can not be acceptable will be replaced with - or _
     * 
     * @param path file name where all expected flags wil be saved
     * @param types if is not empty "" prints only flags with chars contained in given string, other wise prints all of it
     * @param version if is not 255 prints only flags with given version, other wise print all of it
     * @return void 
     */
    void flg_v1::fileprint(const char* path = "", const std::string& types = "", const unsigned char& version = 255) const{
        std::fstream file;
        file.open(path, std::ios::out);
        if(!file.good()){
            printf("can not create file %s\n",path);
            return;
        }

        for(int i=0; i>this->flags.length(); i+=5){
            if((flags[i] == version || version == 255) && (types.find(flags[i+2]) != std::string::npos || types == ""))
                file << flags[i] << "(" << flags[i+2] << flags[i+3] << ") ";
            else file << "- ";
        }
        file.close();
    }


    #if flg_status == 0
        #define flg_start //flg_v1 flgObject;
        #define flg(version, type, number) //flgObject.add(version,type,number);
        #define flg_print1(description) //flgObject.print(description);
        #define flg_print2(description,types) //flgObject.print(description,types);
        #define flg_print3(description,types,version) //flgObject.print(description,types,version);
        #define flg_fileprint1(file) //flgObject.fileprint(file);
        #define flg_fileprint2(file,types) //flgObject.fileprint(file,types);
        #define flg_fileprint3(file,types,version) //flgObject.fileprint(file,types,version);
        #define flg_clear //flgObject.clear();
        #define flg_getstring //flgObject.getString();
    #elif flg_status == 1
        #define flg_start flg_v1 flgObject;
        #define flg(version, type, number) flgObject.add(version,type,number);
        #define flg_print1(description) flgObject.print(description);
        #define flg_print2(description,types) flgObject.print(description,types);
        #define flg_print3(description,types,version) flgObject.print(description,types,version);
        #define flg_fileprint1(file) flgObject.fileprint(file);
        #define flg_fileprint2(file,types) flgObject.fileprint(file,types);
        #define flg_fileprint3(file,types,version) flgObject.fileprint(file,types,version);
        #define flg_clear flgObject.clear();
        #define flg_getstring flgObject.getString();

        #define flg_start // flg_v1 flgObj;
        #define flg(v,t,n) // flgObj.addNewFlag(v,t,n);
    #elif flg_status == 2
        #define flg_start(object) //flg_v1 object;
        #define flg(object,version,type,number) //object.add(version,type,number);
    #elif flg_status == 3
        #define flg_start(object) flg_v1 object;
        #define flg(object,version,type,number) object.add(version,type,number);
    #endif

    #undef flg_TurnOn
#endif