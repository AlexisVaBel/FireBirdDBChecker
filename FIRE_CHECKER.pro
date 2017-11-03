
QMAKE_CXXOUTPUT = -o
QMAKE_CXXFLAGS += -DIBPP_LINUX
QMAKE_CXXFLAGS += -W -Wall -fPIC
QMAKE_CXXFLAGS += -g -DDEBUG
CONFIG+=c++11





HEADERS += \
    ibpp/core/_ibpp.h \
    ibpp/core/ibase.h \
    ibpp/core/iberror.h \
    ibpp/core/ibpp.h \
    ptrChecker/cdbpattern.hpp \
    dbElms/cdbelement.hpp \
    cmn/usedtypes.h \
    loger/logging.h \
    dbInfo/cdbfileinfo.hpp

SOURCES += \
    ibpp/core/_dpb.cpp \
    ibpp/core/_ibpp.cpp \
    ibpp/core/_ibs.cpp \
    ibpp/core/_rb.cpp \
    ibpp/core/_spb.cpp \
    ibpp/core/_tpb.cpp \
    ibpp/core/all_in_one.cpp \
    ibpp/core/array.cpp \
    ibpp/core/blob.cpp \
    ibpp/core/database.cpp \
    ibpp/core/date.cpp \
    ibpp/core/dbkey.cpp \
    ibpp/core/events.cpp \
    ibpp/core/exception.cpp \
    ibpp/core/row.cpp \
    ibpp/core/service.cpp \
    ibpp/core/statement.cpp \
    ibpp/core/time.cpp \
    ibpp/core/transaction.cpp \
    ibpp/core/user.cpp \
    main.cpp \
    ptrChecker/cdbpattern.cpp \
    dbElms/cdbelement.cpp \
    dbInfo/cdbfileinfo.cpp \
    loger/logging.cpp

#for linux building
DEFINES+=IBPP_LINUX=1
INCLUDEPATH+=/opt/firebird/include
LIBS+=-L/opt/firebird/lib -lfbclient
LIBS+=-lboost_log -lboost_thread -lboost_date_time -lboost_system -lboost_filesystem -lpthread -lcrypto  -lssl
#for linux building
