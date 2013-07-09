#pragma once
#include "stdafx.h"
using namespace  cdec;

//--------------------------------------------
/*convert sql::SQLString to cdec::stringx*/
stringx SqlStr2Strx(sql::SQLString sqlStr, ref<Encoding> encode = Encoding::get_UTF8());
/*convert cdec::stringx to sql::SQLString*/
sql::SQLString Strx2SqlStr(stringx strx, ref<Encoding> encode = Encoding::get_UTF8());


class DbUtil: public Object
{
	DECLARE_REF_CLASS(DbUtil)
protected:
	stringx m_dburl;
	stringx m_uname;
	stringx m_pwd;
	stringx m_database;
	stringx m_desc;

	ref<Connection> m_conn;

public:
	DbUtil(stringx pathConfig);
	ref<Connection> Conn();
	ref<Connection> GetConn(){return m_conn;};
	bool Execute(stringx sql);
	int ExecuteUpdate(stringx sql);
	ref<ResultSet> ExecuteQuery(stringx sql);
	void CloseConn();
};

// -------------------------------------------------------------------------- //