GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}Changing type to ASCII${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
TYPE A
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
TYPE A
DONE
EOF
echo "${GREEN}${bold}✔ Cannot access without logging in${NC}${normal}\n"

echo "${GREEN}${bold}Changing type to BINARY${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
TYPE B
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
TYPE B
DONE
EOF
echo "${GREEN}${bold}✔ Cannot access without logging in${NC}${normal}\n"

echo "${GREEN}${bold}Changing type to CONTINUOUS${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
TYPE C
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
TYPE C
DONE
EOF
echo "${GREEN}${bold}✔ Cannot access without logging in${NC}${normal}\n"