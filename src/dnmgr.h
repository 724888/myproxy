#ifndef __DNMGR_H__
#define __DNMGR_H__

#include <memory>
#include "ctnr_impl.h"
#include "threads.h"

/* data node states */
enum dnStats {
  /* data node is invalid */
  s_invalid,

  /* data node is ok and not used */
  s_free,

  /* data node is used, or there's incompleted
   *  transaction on it */
  s_used,
} ;

/* datanode group management */
class dnGroups {
protected:
  /* data node list */
  std::shared_ptr<safeDataNodeList> m_dnGroup ;

  safeStdVector m_freeGroupIds ;

  /* maximun datanode groups count */
  const uint16_t m_maxDnGroup = 20;

  uint16_t m_nDnGroups ;

  /* idle seconds counter */
  int m_idleCount ;

public:
  dnGroups();
  virtual ~dnGroups(void);

public:

  int create_grp_by_conf(void);

  /* get free items of data node group */
  int get_free_group(safeDataNodeList* &nodes, int &gid);

  /* return back the data node group item */
  int return_group(int gid);

  /* get specified datanode group */
  safeDataNodeList* get_specific_group(int);

  int get_num_groups(void) const;

  int acquire_all_groups(void);

  int release_all_groups(void);
} ;

class tDNInfo;
class tTblDetails;
class dnmgr : public dnGroups
{
protected:
  thread_helper<void(dnmgr::*)(int),dnmgr> m_thread;

public:
  dnmgr() ;
  virtual ~dnmgr() ;

protected:

  int get_tables_by_conf(void);

  int get_datanodes_by_conf(void);

  int update_dn_conn(void);

  int new_connection(tDNInfo*);

  int refresh_tbl_info(bool=false);

  int update_tbl_struct(tDNInfo*,tTblDetails*,bool=false);

  int update_tbl_extra_info(tDNInfo*,tTblDetails*,bool=false);

  int add_dn_tbl_relations(auto sch, auto tbl);

  tDNInfo* get_valid_datanode(safeDataNodeList*,tTblDetails*);

  int keep_dn_conn(void);

  int batch_stmt_close(void);

  void datanode_idle_task(int);

public:
  int initialize(void);
} ;

#endif /* __DNMGR_H__*/

