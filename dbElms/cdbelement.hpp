#ifndef CDBELEMENT_H
#define CDBELEMENT_H

#include "../cmn/usedtypes.h"
#include "../loger/logging.h"

class CDBElement
{
public:
    CDBElement(stringT &elmName);
    void    printElm(){TRACE(m_strElmName);}
    bool    operator ==(const CDBElement &elm){
        return ((elm.m_strElmName.compare(m_strElmName))==0);
    }

private:
    stringT    m_strElmName;
};

#endif // CDBELEMENT_H
