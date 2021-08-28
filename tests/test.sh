#! /bin/sh

GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}Login ${NC}${normal}"
../client/client << EOF
USER user1
ACCT u1acc1
PASS pass123
DONE
EOF
echo "${GREEN}${bold}✔ Login Tested${NC}${normal}"

echo "\n"
echo "${GREEN}${bold}Trying withoug login  ${NC}${normal}"
../client/client << EOF
USER user1
ACCT u1acc1
PASS pass123
DONE
EOF