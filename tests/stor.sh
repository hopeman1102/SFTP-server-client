GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}Trying to use STOR without complete arguments${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
STOR
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
STOR
DONE
EOF
echo "${GREEN}${bold}✔ Could not use STOR with incomplete arguments${NC}${normal}\n"

echo "${GREEN}${bold}Using STOR with NEW command${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
STOR NEW stor_test.txt
SIZE 100
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
STOR NEW stor_test.txt
SIZE 100
DONE
EOF
echo "${GREEN}${bold}✔ File successfully transferred.\nTo verify check recieved_files folder in server folder.${NC}${normal}\n"

echo "${GREEN}${bold}Using STOR with OLD command${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
STOR OLD stor_test.txt
SIZE 100
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
STOR OLD stor_test.txt
SIZE 100
DONE
EOF
echo "${GREEN}${bold}✔ File successfully transferred.\nTo verify check recieved_files folder in server folder.${NC}${normal}\n"

echo "${GREEN}${bold}Using STOR with APP command${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
STOR APP stor_test.txt
SIZE 100
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
STOR APP stor_test.txt
SIZE 100
DONE
EOF
echo "${GREEN}${bold}✔ File successfully transferred.\nTo verify check recieved_files folder in server folder.${NC}${normal}\n"