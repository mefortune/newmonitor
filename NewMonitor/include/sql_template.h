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

const wchar_t sql_create_tbl_data[] = LR"sql(
CREATE TABLE "main"."%s" (
	"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	"seg_id" INTEGER NOT NULL,
	"data_time" INTEGER NOT NULL,
	"data" BLOB NOT NULL
)
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

