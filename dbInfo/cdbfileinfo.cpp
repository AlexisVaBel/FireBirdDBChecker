#include "cdbfileinfo.hpp"

#include <system_error>
#include <algorithm>
#include "../loger/logging.h"


stringT STR_RDB_TABLES ="select RDB$RELATION_NAME as tbl from RDB$RELATIONS where (RDB$SYSTEM_FLAG=0)";
stringT STR_RDB_TRIGS  ="select RDB$TRIGGER_NAME ,RDB$RELATION_NAME from RDB$TRIGGERS where RDB$SYSTEM_FLAG=0";
stringT STR_RDB_GENS =  "select RDB$GENERATOR_NAME from RDB$GENERATORS";

/*!
connect params
param[in] filePath - path to db file
param[in] servHost - host on which firebird works
param[in] userName - user to connect with
param[in] password - user`s password
*/
CDBFileInfo::CDBFileInfo(stringT &&servHost, stringT &&filePath, stringT &&userName, stringT &&password):
    m_strHost(servHost),m_strPath(filePath),m_strUser(userName),m_strPass(password){
}

CDBFileInfo::~CDBFileInfo(){
    TRACE(m_strPath+" disconnected");
    disconnectDB();
    for(CDBElement *elmlcl: m_vctElm){
        delete elmlcl;
    }
}

/*!
//  captain Obvious
//  no matter what to compare, tables, triggers or procedures
//  if pattern file has and another not - files not identical
*/
bool CDBFileInfo::cmpDBFiles(CDBFileInfo &dbInfo){
    if(!(connectDB()&& dbInfo.connectDB()))return false;
//    compare tables
    loadTables();
    dbInfo.loadTables();
    if(!compareVct(*(dbInfo.getVct()))){
        TRACE("NOT IDENTICAL TABLES");
        return false;
    }
    TRACE("TABLES OK");
    //compare triggers
    loadTriggers();
    dbInfo.loadTriggers();
    if(!compareVct(*(dbInfo.getVct()))){
        TRACE("NOT IDENTICAL TRIGGERS");
        return false;
    }
    TRACE("TRIGS OK");
    //compare generators
    loadGens();
    dbInfo.loadGens();
    if(!compareVct(*(dbInfo.getVct()))){
        TRACE("NOT IDENTICAL GENS");
        return false;
    }
    TRACE("GENS OK");
//     after all checked
//     we can say, "yeh, structure we need"
    disconnectDB();
    return true;
}

vctDBElmT *CDBFileInfo::getVct(){
    return &m_vctElm;
}

bool CDBFileInfo::existsElm(CDBElement *elmExt){
    for(CDBElement *elmInner: m_vctElm){
        if((*elmInner) == (*elmExt))return true;
    }
    INFO("ELM NOT FOUND ");
    elmExt->printElm();
    return false;
}
//captain Obvious


bool CDBFileInfo::connectDB(){
    m_db=IBPP::DatabaseFactory(m_strHost,m_strPath,m_strUser,m_strPass);
    m_db->Connect();
    TRACE(m_strPath+" connected");
    return m_db->Connected();
}

bool CDBFileInfo::disconnectDB(){
    if(m_db->Connected())m_db->Disconnect();
    return (!m_db->Connected());
}

void CDBFileInfo::loadTables(){
   loadDtToVct(enmTbl);
}

void CDBFileInfo::loadTriggers(){
    loadDtToVct(enmTrig);
}

void CDBFileInfo::loadGens(){
    loadDtToVct(enmGens);
}

void CDBFileInfo::loadDtToVct(enmInfoT whatInfo){
    for(CDBElement *elmlcl: m_vctElm){
        delete elmlcl;
    }
    m_vctElm.clear();
    IBPP::Transaction tr=IBPP::TransactionFactory(m_db,IBPP::amRead,IBPP::ilReadCommitted,IBPP::lrNoWait,IBPP::tfNoAutoUndo);
    IBPP::Statement st=IBPP::StatementFactory(m_db,tr);
    tr->Start();
    if(whatInfo==enmTbl){
        TRACE(m_strPath+" LOADING TABLES");
        st->Execute(STR_RDB_TABLES);
    };
    if(whatInfo==enmGens){
        TRACE(m_strPath+" LOADING GENS");
        st->Execute(STR_RDB_GENS);
    };
    if(whatInfo==enmTrig){
        TRACE(m_strPath+" LOADING TRIGS");
        st->Execute(STR_RDB_TRIGS);
    }
    stringT strIn;
    while(st->Fetch()){
        st->Get(1, strIn);
        m_vctElm.push_back(new CDBElement(strIn));
    }
    tr->Commit();
}


bool CDBFileInfo::compareVct(vctDBElmT vctExt){
    bool bIdent=true;
    if((m_vctElm.size()==0)&&(vctExt.size()!=0)){
        TRACE("different elm cnt");
        return false;
    }
    for(CDBElement *elmExt: vctExt){
        if(!findInVct(m_vctElm,elmExt))return false;
    }
    for(CDBElement *elmExt: m_vctElm){
        if(!findInVct(vctExt,elmExt))return false;
    }
    return bIdent;
}


bool CDBFileInfo::findInVct(vctDBElmT vct1, CDBElement *elmExt){
    for(CDBElement *elmInner: vct1){
        if((*elmInner) == (*elmExt))return true;
    }
    INFO("NOT FOUND ");
    elmExt->printElm();
    return false;
}

bool CDBFileInfo::repairDB(){
    IBPP::Service svc=IBPP::ServiceFactory(m_strHost,m_strUser,m_strPass);
    if(svc==NULL)return false;
    if(!svc->Connected()){
        svc->Connect();
    }
    svc->Repair(m_strPath,IBPP::RPF::rpValidateFull);
    svc->Disconnect();
    return true;
}

//methods for retrieve stat info

void CDBFileInfo::getInfo(){
    int ODS     =0;
    int ODSMinor=0;
    int pageSize=0;
    int pages   =0;
    int buffers =0;
    int sweep   =0;
    bool sync   =false;
    bool reserve=false;
    m_db->Info(&ODS,&ODSMinor,&pageSize,&pages,&buffers,&sweep,&sync,&reserve);
    INFO("***********************************");
    INFO("INFO");
    TRACE("ODS is "+std::to_string(ODS));
    TRACE("ODSMinor is "+std::to_string(ODSMinor));
    TRACE("pageSize is "+std::to_string(pageSize));
    TRACE("pages is "+std::to_string(pages));
    TRACE("buffers "+std::to_string(buffers));
    TRACE("sweep is "+std::to_string(sweep));
    TRACE("sync is "+std::to_string(sync));
    TRACE("reserve is "+std::to_string(reserve));
    INFO("***********************************");
}

void CDBFileInfo::getStat(){
    int fetches =0;
    int marks   =0;
    int reads   =0;
    int writes  =0;
    m_db->Statistics(&fetches,&marks,&reads,&writes);
    INFO("***********************************");
    INFO("STAT");
    TRACE("fetches "+std::to_string(fetches));
    TRACE("marks "+std::to_string(marks));
    TRACE("reads "+std::to_string(reads));
    TRACE("writes "+std::to_string(writes));
    INFO("***********************************");
}

void CDBFileInfo::getCounts(){
    int insert  =0;
    int update  =0;
    int deletes =0;
    int readIdx =0;
    int readSeq =0;
    m_db->Counts(&insert,&update,&deletes,&readIdx,&readSeq);
    INFO("***********************************");
    INFO("COUNTS");
    TRACE("Insert "+std::to_string(insert));
    TRACE("Update "+std::to_string(update));
    TRACE("deletes "+std::to_string(deletes));
    TRACE("readIdx "+std::to_string(readIdx));
    TRACE("readSeq "+std::to_string(readSeq));
    INFO("***********************************");
}
