PRI_PATH = $$PWD

COMMON_TRACE_SRC_PATH = "$${PRI_PATH}"

INCLUDEPATH += \
    "$${COMMON_TRACE_SRC_PATH}/.."

HEADERS += \
    $${COMMON_TRACE_SRC_PATH}/trace.h
