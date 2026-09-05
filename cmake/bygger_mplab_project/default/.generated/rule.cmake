# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "/Users/sindrenavelsaker/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(bygger_mplab_project_default_default_AVR_GCC_assemble_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "${MP_EXTRA_AS_PRE}"
        "-mmcu=atmega162"
        "-B${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/gcc/dev/atmega162"
        "-c"
        "-Wa,--defsym=__MPLAB_BUILD=1${MP_EXTRA_AS_POST}")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__ATmega162__"
        PRIVATE "default=default")
    target_include_directories(${target} PRIVATE "${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/include")
endfunction()
function(bygger_mplab_project_default_default_AVR_GCC_assembleWithPreprocess_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "${MP_EXTRA_AS_PRE}"
        "-mmcu=atmega162"
        "-B${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/gcc/dev/atmega162"
        "-c"
        "-Wa,--defsym=__MPLAB_BUILD=1${MP_EXTRA_AS_POST}")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__ATmega162__"
        PRIVATE "default=default")
    target_include_directories(${target} PRIVATE "${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/include")
endfunction()
function(bygger_mplab_project_default_default_AVR_GCC_compile_rule target)
    set(options
        "-g"
        "-gdwarf-2"
        "${MP_EXTRA_CC_PRE}"
        "-mmcu=atmega162"
        "-B${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/gcc/dev/atmega162"
        "-x"
        "c"
        "-c"
        "-funsigned-char"
        "-funsigned-bitfields"
        "-O1"
        "-ffunction-sections"
        "-fdata-sections"
        "-fpack-struct"
        "-fshort-enums"
        "-Wall")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__ATmega162__"
        PRIVATE "default=default")
    target_include_directories(${target} PRIVATE "${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/include")
endfunction()
function(bygger_mplab_project_default_default_AVR_GCC_compile_cpp_rule target)
    set(options
        "-g"
        "-gdwarf-2"
        "${MP_EXTRA_CC_PRE}"
        "-mmcu=atmega162"
        "-B${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/gcc/dev/atmega162"
        "-x"
        "c++"
        "-c"
        "-funsigned-char"
        "-funsigned-bitfields"
        "-O1"
        "-ffunction-sections"
        "-fdata-sections"
        "-fpack-struct"
        "-fshort-enums"
        "-Wall")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__ATmega162__"
        PRIVATE "default=default")
    target_include_directories(${target} PRIVATE "${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/include")
endfunction()
function(bygger_mplab_project_default_link_rule target)
    set(options
        "-gdwarf-2"
        "${MP_EXTRA_LD_PRE}"
        "-mmcu=atmega162"
        "-B${PACK_REPO_PATH}/Microchip/ATmega_DFP/3.6.299/gcc/dev/atmega162"
        "-Wl,-Map=mem.map"
        "-Wl,--defsym=__MPLAB_BUILD=1${MP_EXTRA_LD_POST}"
        "-Wl,--gc-sections")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__ATmega162__"
        PRIVATE "default=default")
endfunction()
function(bygger_mplab_project_default_objcopy_ihex_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJCOPY}
        ARGS --output-target=ihex ${bygger_mplab_project_default_image_name} ${bygger_mplab_project_default_image_base_name}.hex
        WORKING_DIRECTORY ${bygger_mplab_project_default_output_dir})
endfunction()
function(bygger_mplab_project_default_objcopy_eep_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJCOPY}
        ARGS --only-section=.eeprom --change-section-lma .eeprom=0 --no-change-warnings --output-target=ihex ${bygger_mplab_project_default_image_name} ${bygger_mplab_project_default_image_base_name}.eep
        WORKING_DIRECTORY ${bygger_mplab_project_default_output_dir})
endfunction()
function(bygger_mplab_project_default_objcopy_lss_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJDUMP}
        ARGS --disassemble --wide --demangle --line-numbers --section-headers --source ${bygger_mplab_project_default_image_name} > ${bygger_mplab_project_default_image_base_name}.lss
        WORKING_DIRECTORY ${bygger_mplab_project_default_output_dir})
endfunction()
function(bygger_mplab_project_default_objcopy_srec_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJCOPY}
        ARGS --output-target=srec --remove-section=.eeprom --remove-section=.fuse --remove-section=.lock --remove-section=.signature ${bygger_mplab_project_default_image_name} ${bygger_mplab_project_default_image_base_name}.srec
        WORKING_DIRECTORY ${bygger_mplab_project_default_output_dir})
endfunction()
function(bygger_mplab_project_default_objcopy_sig_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJCOPY}
        ARGS --only-section=.user_signatures --change-section-lma .user_signatures=0 --no-change-warnings --output-target=ihex ${bygger_mplab_project_default_image_name} ${bygger_mplab_project_default_image_base_name}.usersignatures
        WORKING_DIRECTORY ${bygger_mplab_project_default_output_dir})
endfunction()
function(bygger_mplab_project_default_objcopy_memusage_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJDUMP}
        ARGS -Pmem-usage ${bygger_mplab_project_default_image_name}
        WORKING_DIRECTORY ${bygger_mplab_project_default_output_dir})
endfunction()
