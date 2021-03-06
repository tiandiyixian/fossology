/*********************************************************************
Copyright (C) 2011 Hewlett-Packard Development Company, L.P.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************/

/* cunit includes */
#include <CUnit/CUnit.h>
#include "delagent.h"
#include <string.h>

extern char *DBConfFile;
static PGresult *result = NULL;

/**
 * \file testDeleteFolders.c
 * \brief testing for the function DeleteFolders and DeleteUploads
 */

/**
 * \brief for function DeleteFolders
 * \test
 * -# Give a folder id to deleteFolder()
 * -# Check for return code
 */
void testDeleteFolders()
{
  long FolderId = 3;
  //char *DBConfFile = NULL;  /* use default Db.conf */
  char *ErrorBuf;
  int rc;

  db_conn = fo_dbconnect(DBConfFile, &ErrorBuf);
  /** exectue the tested function */
  rc = deleteFolder(3, FolderId, 3, 10);

  PQfinish(db_conn);
  CU_ASSERT_EQUAL(rc, 0);
  CU_PASS("DeleteFolders PASS!");
}

/**
 * \brief for function DeleteUploads
 * \test
 * -# Delete an upload using deleteUpload() and check for the return
 * -# Check if the upload is delete from database
 * -# Check if the copyrights for the given upload is also deleted
 */
void testDeleteUploads()
{
  long UploadId = 2;
  //char *DBConfFile = NULL;  /* use default Db.conf */
  char *ErrorBuf;
  char sql[1024];
  int rc;

  db_conn = fo_dbconnect(DBConfFile, &ErrorBuf);
  /** exectue the tested function */
  rc = deleteUpload(UploadId, 3, 10);
  CU_ASSERT_EQUAL(rc, 0);

  /* check if uploadtree records deleted */
  memset(sql, '\0', 1024);
  snprintf(sql, 1024, "SELECT * FROM uploadtree WHERE upload_fk = %ld;", UploadId);
  result = PQexec(db_conn, sql);
  if (fo_checkPQresult(db_conn, result, sql, __FILE__, __LINE__))
  {
    CU_FAIL("DeleteUploads FAIL!");
  }
  else
  {
    CU_ASSERT_EQUAL(PQntuples(result),0);
  }
  PQclear(result);

  /* check if copyright records deleted */
  memset(sql, '\0', 1024);
  snprintf(sql, 1024, "SELECT * FROM copyright C INNER JOIN uploadtree U ON C.pfile_fk = U.pfile_fk AND U.upload_fk = %ld;", UploadId);
  result = PQexec(db_conn, sql);
  if (fo_checkPQresult(db_conn, result, sql, __FILE__, __LINE__))
  {
    CU_FAIL("DeleteUploads FAIL!");
  }
  else
  {
    CU_ASSERT_EQUAL(PQntuples(result),0);
  }
  PQclear(result);

  /** Check false input */
  UploadId = 4;
  rc = deleteUpload(UploadId, 2, 10);
  CU_ASSERT_NOT_EQUAL(rc, 0);

  PQfinish(db_conn);
  CU_PASS("DeleteUploads PASS!");
}

/**
 * \brief for function DeleteLicenses
 * \test
 * -# Delete licenses for a given upload using deleteLicense()
 * -# Check for the return code
 * \todo Fix issue 1057
 */
void testDeleteLicenses()
{
  long UploadId = 2;
  char *ErrorBuf;
  int rc;

  db_conn = fo_dbconnect(DBConfFile, &ErrorBuf);
  /** exectue the tested function */
  rc = deleteLicense(UploadId, 10);

  PQfinish(db_conn);
  CU_ASSERT_EQUAL(rc, 0);
  CU_PASS("DeleteLicenses PASS!");
}

/**
 * \brief testcases for function Delete
 */
CU_TestInfo testcases_DeleteFolders[] =
{
#if 0
#endif
{"Testing the function DeleteFolders:", testDeleteFolders},
{"Testing the function DeleteUploads:", testDeleteUploads},
// TODO fix #1057 {"Testing the function DeleteLicenses:", testDeleteLicenses},
  CU_TEST_INFO_NULL
};

