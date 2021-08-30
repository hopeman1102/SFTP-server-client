GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}LISTING all files using LIST with F mode${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
LIST F
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
LIST F
DONE
EOF
echo "${GREEN}${bold}✔ Files in current directory listed with F mode${NC}${normal}\n"

echo "${GREEN}${bold}LISTING all files using LIST with V mode${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
LIST V
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
LIST V
DONE
EOF
echo "${GREEN}${bold}✔ Files in current directory listed with V mode${NC}${normal}\n"

echo "${GREEN}${bold}Specifying subdir while using LIST with V mode${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
LIST V dir1
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
LIST V dir1
DONE
EOF
echo "${GREEN}${bold}✔ Files in directory dir1 listed with V mode${NC}${normal}\n"

echo "${GREEN}${bold}Changing directory using CDIR and listing${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
CDIR dir1
LIST V
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
CDIR dir1
LIST V
DONE
EOF
echo "${GREEN}${bold}✔ Files in changed directory listed${NC}${normal}\n"