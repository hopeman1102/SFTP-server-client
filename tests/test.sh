#! /bin/sh

GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "-------------DATABASE-------------"
echo " ---id----|---acc----|---pass---- "
echo " --user1--|--u1acc1--|--pass123-- "
echo " --user2--|--u2acc1--|--pass123-- "
echo " --user2--|--u2acc1--|--pass123-- "
echo " --user3--|---NULL---|---NULL---- "
echo " --user4--|--u4acc1--|---NULL---- "
echo " --user5--|---NULL---|--pass123-- "
echo "----------------------------------\n"

echo "${bold}Run the server in a different terminal from server directory before this test.\nFrom the main directory -> cd server -> ./server${NC}\n"

echo "${GREEN}${bold}Full Login${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user1
ACCT u1acc1
PASS pass123
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user1
ACCT u1acc1
PASS pass123
DONE
EOF
echo "${GREEN}${bold}✔ Login Tested${NC}${normal}\n"

echo "${GREEN}${bold}Login when PASS and ACCT not required${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user3
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user3
DONE
EOF
echo "${GREEN}${bold}✔ Login without PASS and ACCT Tested${NC}${normal}\n"

echo "${GREEN}${bold}Login when PASS not required${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user4
ACCT u4acc1
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user4
ACCT u4acc1
DONE
EOF
echo "${GREEN}${bold}✔ Login without PASS Tested${NC}${normal}\n"

echo "${GREEN}${bold}Login when ACCT not required${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
USER user5
PASS pass123
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
USER user5
PASS pass123
DONE
EOF
echo "${GREEN}${bold}✔ Login without ACCT Tested${NC}${normal}\n"

echo "${GREEN}${bold}Trying to run a command without logging in${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
TYPE A
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
TYPE A
DONE
EOF
echo "${GREEN}${bold}✔ Cannot access without logging in${NC}${normal}\n"

echo "${GREEN}${bold}Trying to run a command without logging in${NC}${normal}"
echo "${GREEN}Commands from client:${NC}"
cat << EOF
TYPE A
DONE
EOF
echo "${GREEN}Message from Server:${NC}"
../client/client << EOF
TYPE A
DONE
EOF
echo "${GREEN}${bold}✔ Cannot access without logging in${NC}${normal}\n"