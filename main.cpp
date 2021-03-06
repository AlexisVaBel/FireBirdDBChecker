#include "loger/logging.h"
#include "dbInfo/cdbfileinfo.hpp"
#include "getopt.h"

// example
//-H localhost -P /work/cmn/db/mznkrp2/MZNKRP2MES.FDB -p /work/cmn/db/mznkrp2/MZNKRP2MES2.FDB

static const char *optStrings="H:P:U:W:h:p:u:w:IiR";
static const int DB_OK    = 0;
static const int DB_DIFFS = 1;
static const int DB_ERR   = 2;
static const int DB_NPROCS= 3; // not processed


struct globalArgs_t{
    char *host1;    /*-H*/
    char *path1;    /*-P*/
    char *user1;    /*-U*/
    char *pass1;    /*-W*/
    char *host2;    /*h*/
    char *path2;    /*p*/
    char *user2;    /*u*/
    char *pass2;    /*w*/    
    bool repair;    /*R*/
    bool silent;    /*S*/
}globalArgs;

void initArgs(){
    globalArgs.host1=(char *)"";
    globalArgs.path1=(char *)"";
    globalArgs.user1=(char *)"";
    globalArgs.pass1=(char *)"";

    globalArgs.host2=(char *)"";
    globalArgs.path2=(char *)"";
    globalArgs.user2=(char *)"";
    globalArgs.pass2=(char *)"";
    globalArgs.repair=false;
    globalArgs.silent=true;
}


void fillArgs(int opt){
    switch(opt){
        case 'H':globalArgs.host1=optarg;
        break;
        case 'P':globalArgs.path1=optarg;
        break;
        case 'U':globalArgs.user1=optarg;
        break;
        case 'W':globalArgs.pass1=optarg;
        break;
        case 'h':globalArgs.host2=optarg;
        break;
        case 'p':globalArgs.path2=optarg;
        break;
        case 'u':globalArgs.user2=optarg;
        break;
        case 'w':globalArgs.pass2=optarg;
        break;
        case 'i':
        case 'I':{
        TRACE("********************* usage ***********************");
        TRACE("*!!!!!!!!!!!!!!!!!!!! must be !!!!!!!!!!!!!!!!!!!!*");
        TRACE("* -H host1 -P path1 -h(if need host2) -p path2    *");
        TRACE("*path2 may be empty, use next command to repair db*");
        TRACE("* -R                                              *");
        TRACE("*                  not nescessary                 *");
        TRACE("* use than have different user and password       *");
        TRACE("* -U user1 -u user2 -W password1 -w password2     *");
        TRACE("********************//usage ***********************");
        }
        break;
        case 'R':
        globalArgs.repair=true;
        break;
        case 'S':
        globalArgs.silent=true;
        break;

    }
}



void printLabel(){
    TRACE(".::::'`");
    TRACE(": :::::'");
    TRACE(".::::::.::::::::::");
    TRACE("..:::::``::::::::");
    TRACE(",cC'':::::: .:::::::::::::");
    TRACE("\"?$$$$P'::::::::::::::::::::");
    TRACE("'':::::::::::::::::::::::");
    TRACE(".::::::::::: d$c`:`,c:::");
    TRACE("`::::::::::: $$$$, $$ :");
    TRACE(",,,,```:::::: F \"$'  ?::  ..:::::::::::..");
    TRACE(",d$$$$$$$$$$c,` '  ?::   . :::::::::::::::::");
    TRACE("d$$$$$$$$$$$$$$$$$$$c$F,d$$F::::::::::::::::::");
    TRACE("d$$$$$$$$$$$F?$$$$$$$$$.$$$$$ ::::::::::::::::::.");
    TRACE(",$$????$$$$$$$$$hccc-$$$$$$$$$$,::::::::::::::::',$");
    TRACE("'    4$$$$$$$$$$$$$$,\"?$$$$$$$$$c`:::::::::::'',$\"");
    TRACE("$$$\"\".$$$$$$$$$$$L`$$$$$$$$$$$bccc,ccc$$$$\"");
    TRACE("\"      ::: `?$$$$$$,??$$$$$$$$$$P\"????\"");
    TRACE(":::: `$$$$$$");
    TRACE(":::::`$d$");
    TRACE(":: :?$$$::::");
    TRACE(":::'.:4$$$'.:::");
    TRACE("::: ::4$$$$$ :`::");
    TRACE(":::`: $$$$$$L::`:");
    TRACE("`:::::?$$$$$$<: :");
    TRACE(",,  ``  :::\"$$$$ ::''");
    TRACE("$$??\" =4- ,,`::\"?::,c='? Lcdbc,");
    TRACE("c$$$$\",$$$$$c\"'  'dLd$$$$b,?$$$$");
    TRACE("??$$$$,??$$$$$$     $$$$$c $ $$ J");
    TRACE("\"?$$$$3`z$$$$P\"     \"?$$$$P\" \"    ");
}

// error codes
// 1 app crashed not enough arguments
// 2 err mismatch tables


void readConf(int argc, char** argv){
    int opt=0;
    initArgs();
    TRACE("======getting args======");
    do{
        opt=getopt(argc,argv,optStrings);
        fillArgs(opt);
    }while(opt!=-1);
}


int main(int argc, char** argv){
//    setLoggingForDaemon();
    int iReturn = DB_NPROCS;
    printLabel();
    readConf(argc, argv);

    if((globalArgs.host1!=nullptr)&&(globalArgs.host1[0]=='\0')){
        TRACE("======NO HOST1 quit======");
        return 0;
    };
    if((globalArgs.path1!=nullptr)&&(globalArgs.path1[0]=='\0')){
        TRACE("======NO PATH1 quit======");
        return 0;
    };
    if((globalArgs.user1!=nullptr)&&(globalArgs.user1[0]=='\0')){
        globalArgs.user1=(char*)"SYSDBA";
    };
    if((globalArgs.pass1!=nullptr)&&(globalArgs.pass1[0]=='\0')){
        globalArgs.pass1=(char*)"masterskey";
    };

    if((globalArgs.host2!=nullptr)&&(globalArgs.host2[0]=='\0')){
        globalArgs.host2=globalArgs.host1;
    };
    if((globalArgs.path2!=nullptr)&&(globalArgs.path2[0]=='\0')){
        TRACE("======NO PATH2  quit======");
        if(!globalArgs.repair)return 0;
    };
    if((globalArgs.user2!=nullptr)&&(globalArgs.user2[0]=='\0')){
        globalArgs.user2=globalArgs.user1;
    };
    if((globalArgs.pass2!=nullptr)&&(globalArgs.pass2[0]=='\0')){
        globalArgs.pass2=globalArgs.pass1;
    };

    if(globalArgs.silent)
        setLoggingForDaemon();


    TRACE("======args ok======");
    std::shared_ptr<CDBFileInfo> pntTargetFile(new CDBFileInfo(globalArgs.host1,globalArgs.path1,globalArgs.user1,globalArgs.pass1));

    try{
    if(!globalArgs.repair){
        iReturn = DB_OK;
        std::shared_ptr<CDBFileInfo> pntTemplateFile(new CDBFileInfo(globalArgs.host2,globalArgs.path2,globalArgs.user2,globalArgs.pass2));
        if(pntTargetFile.get()->cmpDBFiles(*pntTemplateFile.get()))
            TRACE("======DBASES identical======");
        else{
            TRACE("=====DBASES NOT identical===");
            iReturn=DB_DIFFS;
        }
    }else{
        iReturn = DB_OK;
        TRACE("repair DB");
        if(pntTargetFile.get()->repairDB())
            TRACE("seems to be repaired DB");
        else{
            TRACE("not repaired DB");
            iReturn = DB_ERR;
        }
    }
    }catch (IBPP::Exception &e){
        TRACE(e.ErrorMessage());
    }

    return iReturn;
}
