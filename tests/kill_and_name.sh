GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}Trying to KILL while specifying wrong name${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
KILL wrongname.txt
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
KILL wrongname.txt
DONE
EOF
echo "${GREEN}${bold}✔ File not deleted as it does not exist${NC}${normal}\n"

echo "${GREEN}${bold}Trying to KILL while specifying correct name${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
KILL deleteThis.txt
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
KILL deleteThis.txt
DONE
EOF
echo "${GREEN}${bold}✔ deleteThis.txt file deleted in the server${NC}${normal}\n"

echo "${GREEN}${bold}Trying to rename with NAME while specifying wrong file name${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
NAME wrongRenameMe.txt
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
NAME wrongRenameMe.txt
DONE
EOF
echo "${GREEN}${bold}✔ Cannot rename using NAME as specified filename is wrong${NC}${normal}\n"

echo "${GREEN}${bold}Trying to rename with NAME while specifying correct file name${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
NAME renameMe.txt
TOBE renamed.txt
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
NAME renameMe.txt
TOBE renamed.txt
DONE
EOF
echo "${GREEN}${bold}✔ File renameMe.txt renamed to renamed.txt.\nThis can be confirmed by checking the recieved_files directory in the sever folder.${NC}${normal}\n"