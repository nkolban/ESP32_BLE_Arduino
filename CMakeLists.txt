set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCDIRS src)
list(APPEND COMPONENT_REQUIRES bt nvs_flash)
register_component()
