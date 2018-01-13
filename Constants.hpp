#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace NConstants
{

static constexpr const char * const DATABASE_FILE    = ".zclip.db";
static constexpr const char * const SETTINGS_FILE    = ".zclip";
static constexpr const char * const TABLE_NAME       = "clipboard";

static constexpr const char * const DEFAULT_SETTINGS = \
      "{\"shortcut\":\"Alt+X\"}";

static constexpr const char * const DATABASE_CREATE_QUERY = \
      "CREATE TABLE IF NOT EXISTS `%1` ("
      "`id`    INTEGER   PRIMARY KEY NOT NULL,"
      "`data`  TEXT      UNIQUE      NOT NULL,"
      "`stamp` TIMESTAMP DEFAULT     CURRENT_TIMESTAMP"
      ");";

static constexpr const char * const DATABASE_INSERT_TEXT_QUERY = \
      "INSERT OR REPLACE INTO `%1` (`data`) VALUES (?)";

static constexpr const char * const DATABASE_SELECT_TEXT_QUERY = \
      "SELECT data FROM `%1` WHERE id = ?";

static constexpr const char * const DATABASE_COUNT_QUERY = \
      "SELECT COUNT(*) as count FROM `%1`";

static constexpr const char * const TOOLTIP_TEXT_TEMPLATE = \
      "Clipboard manager:\n"
      " - shortcut key: %1\n"
      " - record in database: %2";

static constexpr const char * const ICON_MAIN = \
      ":/resources/icon/clipboard.svg";

}

#endif // CONSTANTS_HPP
