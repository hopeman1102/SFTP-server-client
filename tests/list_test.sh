GREEN='\033[0;32m'
NC='\033[0m' # No Color
bold=$(tput bold)
normal=$(tput sgr0)

echo "${GREEN}${bold}LISTING all files with F mode${NC}${normal}"
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