
#ifdef _DEBUG

#ifndef _HEAD_DEBUG_
#define _HEAD_DEBUG_
#include <iostream>
#include <string>
#define PNT(s) std::cout<<s<<std::endl;

#define DBGCLASS ,public dbg_class
#define FDBGCLASS :public dbg_class

class dbg_class
{
public:
    std::string dbg_info;
    dbg_class(){std::cout<<this<<"  Created\n";}
    ~dbg_class(){std::cout<<this<<"Killed   info:"<<dbg_info<<std::endl;}
};

#define DBGCLASS_SETDBGINFO(s,t) (s).dbg_info = t;
#define DBGCLASS_PNTDBGINFO(s) cout<<s.dbg_info;

#endif // _HEAD_DEBUG_

#else
#define PNT(s)
#define DBGCLASS
#define FDBGCLASS
#define DBGCLASS_SETDBGINFO(s,t)
#define DBGCLASS_PNTDBGINFO(s)

#endif // _DEBUG
