const wchar_t sql_create_tbl_info[] = LR"sql(
CREATE TABLE
IF NOT EXISTS "main"."TableInfo" (
	"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	"table_name" TEXT (100) NOT NULL,
	"display_name" TEXT (100) NOT NULL,
	"create_time" INTEGER NOT NULL,
	"description" TEXT (100)
)
)sql";

const wchar_t sql_enum_tbl_info[] = LR"sql(
SELECT
	TableInfo.id,
	TableInfo.table_name,
	TableInfo.display_name,
	TableInfo.create_time,
	TableInfo.description
FROM
	TableInfo
)sql";

const wchar_t sql_template_addnew[] = LR"sql(
INSERT INTO "main"."TableInfo" (
	"table_name",
	"display_name",
	"create_time",
	"description"
)
VALUES
	(?1, ?2, ?3, ?4)
)sql";

const wchar_t sql_alter_tbl_info[] = LR"sql(
UPDATE "main"."TableInfo"
SET "display_name"=?1,
	"description" =?2
WHERE
	id=?3
)sql";

const wchar_t sql_delete_tbl_info[] = LR"sql(
DELETE FROM
	"main"."TableInfo"
WHERE
	id=?1
AND "table_name"=?2
)sql";

const wchar_t sql_create_tbl_data[] = LR"sql(
CREATE TABLE 
IF NOT EXISTS "main"."%s" (
	"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	"seg_id" INTEGER NOT NULL,
	"data_time" INTEGER NOT NULL,
	"data" BLOB NOT NULL
)
)sql";

const wchar_t sql_delete_tbl_data[] = LR"sql(
DROP TABLE
IF EXISTS "main"."%s"
)sql";

const wchar_t sql_insert_tbl_data[] = LR"sql(
INSERT INTO "main"."%s" (
	"seg_id",
	"data_time",
	"data"
)
VALUES
	(?1, ?2, ?3)
)sql";

const wchar_t sql_enum_tbl_data[] = LR"sql(
SELECT
	%s.id,
	%s.seg_id,
	%s.data_time,
	%s.data
FROM
	%s
)sql";

