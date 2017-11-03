#include "loger/logging.h"
#include "dbInfo/cdbfileinfo.hpp"
#include "getopt.h"

// example
//-H localhost -P /work/cmn/db/mznkrp2/MZNKRP2MES.FDB -p /work/cmn/db/mznkrp2/MZNKRP2MES2.FDB

static const char *optStrings="H:P:U:W:h:p:u:w:IiR";

struct globalArgs_t{
    char *host1;    /*-H*/
    char *path1;    /*-P*/
    char *user1;    /*-U*/
    char *pass1;    /*-W*/
    char *host2;    /*h*/
    char *path2;    /*p*/
    char *user2;    /*u*/
    char *pass2;    /*w*/
    bool repair;    /*w*/
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
        TRACE("usage: -H host1 -P path1 -h(if need host2) -p path2");
        }
        break;
        case 'R':
        globalArgs.repair=true;
        break;

    }
}


// error codes
// 1 app crashed not enough arguments
// 2 err mismatch tables

int main(int argc, char** argv){
//    setLoggingForDaemon();
    int opt=0;
    int iReturn=0;
    initArgs();
    do{        
        opt=getopt(argc,argv,optStrings);       
        fillArgs(opt);
    }while(opt!=-1);

    CDBFileInfo *fileInfo=NULL;
    CDBFileInfo *fileInfo2=NULL;

    if((globalArgs.host1!=NULL)&&(globalArgs.host1[0]=='\0')){
        TRACE("======NO HOST1 quit======");
        return 0;
    };
    if((globalArgs.path1!=NULL)&&(globalArgs.path1[0]=='\0')){
        TRACE("======NO PATH1 quit======");
        return 0;
    };
    if((globalArgs.user1!=NULL)&&(globalArgs.user1[0]=='\0')){
        globalArgs.user1=(char*)"SYSDBA";
    };
    if((globalArgs.pass1!=NULL)&&(globalArgs.pass1[0]=='\0')){
        globalArgs.pass1=(char*)"masterkey";
    };

    if((globalArgs.host2!=NULL)&&(globalArgs.host2[0]=='\0')){
        globalArgs.host2=globalArgs.host1;
    };
    if((globalArgs.path2!=NULL)&&(globalArgs.path2[0]=='\0')){
        TRACE("======NO PATH2  quit======");
        if(!globalArgs.repair)return 0;
    };
    if((globalArgs.user2!=NULL)&&(globalArgs.user2[0]=='\0')){
        globalArgs.user2=globalArgs.user1;
    };
    if((globalArgs.pass2!=NULL)&&(globalArgs.pass2[0]=='\0')){
        globalArgs.pass2=globalArgs.pass1;
    };


    fileInfo=new CDBFileInfo(globalArgs.host1,globalArgs.path1,globalArgs.user1,globalArgs.pass1);
    if(!globalArgs.repair){
        fileInfo2=new CDBFileInfo(globalArgs.host2,globalArgs.path2,globalArgs.user2,globalArgs.pass2);

        if(fileInfo->cmpDBFiles(*fileInfo2))
            TRACE("======DBASES identical======");
        else{
            TRACE("=====DBASES NOT identical===");
            iReturn=2;
        }
        delete fileInfo2;
    }else{
        TRACE("repair DB");
        fileInfo->repairDB();
    }
//    delete fileInfo;


    // some group of return values if something wrong
    return 0;
}
