#ifndef DBFILEINFO_H
#define DBFILEINFO_H

#include <vector>

#include "ibpp/core/ibpp.h"
#include "../cmn/usedtypes.h"
#include "../dbElms/cdbelement.hpp"

/**
 * \class cdbfileinfo
 *
 * \ingroup dbInfo
 *
 *
 * \brief basic class for loading db structure
 * will be used with pattern db file and working db file
 * for checking if working db is currepted after restore
 *
 * \note
 *
 * \author A.Beljaev
 *
 * \version $Revision: 1.0 $
 *
 * \date $Date: 2017/04/18 10:00 $
 *
 * Contact: alexisvabel@gmail.com
 *
 *
 */

typedef std::vector<CDBElement *> vctDBElmT;
typedef enum{
    enmTbl=0,
    enmGens,
    enmTrig,
    enmProc,
    enmMods
}enmInfoT;
class CDBFileInfo
{
public:

    CDBFileInfo( stringT &&servHost,stringT &&filePath, stringT &&userName, stringT &&password);
    ~CDBFileInfo();    
    bool    disconnectDB();
    bool    cmpDBFiles(CDBFileInfo &dbInfo);
    bool    repairDB();
protected:
    bool    existsElm(CDBElement *elmExt);
    vctDBElmT *getVct();
private:
    vctDBElmT     m_vctElm;

    stringT m_strPath;
    stringT m_strHost;
    stringT m_strUser;
    stringT m_strPass;

    IBPP::Database m_db;

    bool    connectDB();

    void    getInfo();
    void    getStat();
    void    getCounts();



    void    loadTables();
    void    loadTriggers();
    void    loadGens();


    void    loadDtToVct(enmInfoT whatInfo);
    bool    ifDirExists();

    bool    compareVct(vctDBElmT vctExt);
    bool    findInVct(vctDBElmT vct1, CDBElement *elmExt);





};

#endif // DBFILEINFO_H
