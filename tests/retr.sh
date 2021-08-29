GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}Trying to use RETR command when no file is specified${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
RETR
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
RETR
DONE
EOF
echo "${GREEN}${bold}✔ Could not use RETR with incomplete arguments${NC}${normal}\n"

echo "${GREEN}${bold}Trying to use RETR when non existing file is specified${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
RETR abc
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
RETR abc
DONE
EOF
echo "${GREEN}${bold}✔ RETR does not work when file does not exist in the server${NC}${normal}\n"

echo "${GREEN}${bold}Trying to use RETR with correct filename${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
RETR temp1.txt
SEND
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
RETR temp1.txt
SEND
DONE
EOF
echo "${GREEN}${bold}✔ File successfully transferred from server to client.\nThis can be confirmed by observing the transfer_files folder in the tests folder.\nIt can be seen the file transferred is 512 bytes.${NC}${normal}\n"

echo "${GREEN}${bold}Aborting using STOP${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
RETR temp1.txt
STOP
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
RETR temp1.txt
STOP
DONE
EOF
echo "${GREEN}${bold}✔ Successfully aborted using STOP.${NC}${normal}\n"