BIN_ROOT=".."
SRC_ROOT="../../.."
DES_PATH="./test_bin"
BIN_TYPE="Release"

rm -rf ${DES_PATH}

mkdir ${DES_PATH}

mkdir \
${DES_PATH}/test_JSONCPP \
${DES_PATH}/application_manager_test \
${DES_PATH}/data_resumption_test \
${DES_PATH}/message_helper_test \
${DES_PATH}/state_controller_test \
${DES_PATH}/config_profile_test \
${DES_PATH}/connection_handler_test \
${DES_PATH}/formatters_test \
${DES_PATH}/hmi_message_handler_test \
${DES_PATH}/media_manager_test \
${DES_PATH}/policy_test \
${DES_PATH}/protocol_handler_test \
${DES_PATH}/resumption_test \
${DES_PATH}/rpc_base_test \
${DES_PATH}/security_manager_test \
${DES_PATH}/smart_object_test \
${DES_PATH}/transport_manager_test \
${DES_PATH}/utils_test

#test_JSONCPP
cp ${BIN_ROOT}/src/3rd_party-static/test/${BIN_TYPE}/test_JSONCPP.exe ${DES_PATH}/test_JSONCPP
#application_manager_test
cp ${BIN_ROOT}/src/components/application_manager/test/${BIN_TYPE}/application_manager_test.exe ${DES_PATH}/application_manager_test
cp ${BIN_ROOT}/src/components/application_manager/test/hmi_capabilities.json ${DES_PATH}/application_manager_test
cp ${BIN_ROOT}/src/components/application_manager/test/smartDeviceLink_test.ini ${DES_PATH}/application_manager_test
#data_resumption_test
cp ${BIN_ROOT}/src/components/application_manager/test/${BIN_TYPE}/data_resumption_test.exe ${DES_PATH}/data_resumption_test
cp ${BIN_ROOT}/src/components/application_manager/test/hmi_capabilities.json ${DES_PATH}/data_resumption_test
cp ${BIN_ROOT}/src/components/application_manager/test/smartDeviceLink_test.ini ${DES_PATH}/data_resumption_test
#message_helper_test
cp ${BIN_ROOT}/src/components/application_manager/test/message_helper/${BIN_TYPE}/message_helper_test.exe ${DES_PATH}/message_helper_test
#state_controller_test
cp ${BIN_ROOT}/src/components/application_manager/test/state_controller/${BIN_TYPE}/state_controller_test.exe ${DES_PATH}/state_controller_test
cp ${SRC_ROOT}/lib/wince/libeay32.dll ${DES_PATH}/state_controller_test
#config_profile_test
cp ${BIN_ROOT}/src/components/config_profile/test/${BIN_TYPE}/config_profile_test.exe ${DES_PATH}/config_profile_test
cp ${BIN_ROOT}/src/components/config_profile/test/*.ini ${DES_PATH}/config_profile_test
#connection_handler_test
cp ${BIN_ROOT}/src/components/connection_handler/test/${BIN_TYPE}/connection_handler_test.exe ${DES_PATH}/connection_handler_test
cp ${SRC_ROOT}/lib/wince/libeay32.dll ${DES_PATH}/connection_handler_test
cp ${SRC_ROOT}/lib/wince/msvcr90.dll ${DES_PATH}/connection_handler_test
cp ${BIN_ROOT}/src/components/connection_handler/test/*.ini ${DES_PATH}/connection_handler_test
#formatters_test
cp ${BIN_ROOT}/src/components/formatters/test/${BIN_TYPE}/formatters_test.exe ${DES_PATH}/formatters_test
#hmi_message_handler_test
cp ${BIN_ROOT}/src/components/hmi_message_handler/test/${BIN_TYPE}/hmi_message_handler_test.exe ${DES_PATH}/hmi_message_handler_test
#media_manager_test
cp ${BIN_ROOT}/src/components/media_manager/test/${BIN_TYPE}/media_manager_test.exe ${DES_PATH}/media_manager_test
#policy_test
cp ${BIN_ROOT}/src/components/policy/test/${BIN_TYPE}/policy_test.exe ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/src/policy/${BIN_TYPE}/Policy.dll ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/PTU.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/ptu_requestType.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/PTU2.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/ptu2_requestType.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/PTU3.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/PTU4.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/sdl_preloaded_pt.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/sdl_pt_first_update.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/sdl_pt_second_update.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/sdl_pt_update.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/valid_sdl_pt_update.json ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/smartDeviceLink.ini ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/smartDeviceLink2.ini ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/smartDeviceLink3.ini ${DES_PATH}/policy_test
cp ${BIN_ROOT}/src/components/policy/test/log4cxx.properties ${DES_PATH}/policy_test
#protocol_handler_test
cp ${BIN_ROOT}/src/components/protocol_handler/test/${BIN_TYPE}/protocol_handler_test.exe ${DES_PATH}/protocol_handler_test
#resumption_test
cp ${BIN_ROOT}/src/components/resumption/test/${BIN_TYPE}/resumption_test.exe ${DES_PATH}/resumption_test
#rpc_base_test
cp ${BIN_ROOT}/src/components/rpc_base/test/${BIN_TYPE}/rpc_base_test.exe ${DES_PATH}/rpc_base_test
#security_manager_test
cp ${BIN_ROOT}/src/components/security_manager/test/${BIN_TYPE}/security_manager_test.exe ${DES_PATH}/security_manager_test
cp ${SRC_ROOT}/lib/wince/libeay32.dll ${DES_PATH}/security_manager_test
cp ${SRC_ROOT}/lib/wince/ssleay32.dll ${DES_PATH}/security_manager_test
cp ${BIN_ROOT}/src/components/security_manager/test/*.pem ${DES_PATH}/security_manager_test
if [ ! -d "client" ]; then
  python ${SRC_ROOT}/tools/Utils/generate_test_certificates.py
fi
cp -rf client ${DES_PATH}/security_manager_test
cp -rf server ${DES_PATH}/security_manager_test

#smart_object_test
cp ${BIN_ROOT}/src/components/smart_objects/test/${BIN_TYPE}/smart_object_test.exe ${DES_PATH}/smart_object_test
#transport_manager_test
cp ${BIN_ROOT}/src/components/transport_manager/test/${BIN_TYPE}/transport_manager_test.exe ${DES_PATH}/transport_manager_test
cp ${BIN_ROOT}/src/components/transport_manager/test/*.ini ${DES_PATH}/transport_manager_test
cp ${BIN_ROOT}/bin/msvcr90.dll ${DES_PATH}/transport_manager_test
#utils_test
cp ${BIN_ROOT}/src/components/utils/test/${BIN_TYPE}/utils_test.exe ${DES_PATH}/utils_test
cp ${BIN_ROOT}/src/components/utils/test/*.ini ${DES_PATH}/utils_test
cp ${BIN_ROOT}/src/components/utils/test/log4cxx.properties ${DES_PATH}/utils_test