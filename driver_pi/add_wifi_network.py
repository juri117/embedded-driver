
import sys

ssid = "DiAvEn"
pw = "Auto123!"

if len(sys.argv) != 3:
    print('wrong number of parameter, requires ssid and pw as strings')
    sys.exit(-7)
if not isinstance(sys.argv[1], str):
    print('parameter 1: ssid needs to be from type string')
    sys.exit(-4)
if not isinstance(sys.argv[2], str):
    print('parameter 2: pw needs to be from type string')
    sys.exit(-5)

ssid = sys.argv[1]
pw = sys.argv[2]

wifi_config_file = "/etc/wpa_supplicant/wpa_supplicant.conf"

f = open(wifi_config_file, "r")
lines = f.readlines()
f.close()

net_exists = False
current_block = False
for i in range(0, len(lines)):
    if 'ssid' in lines[i] and '"{}"'.format(ssid) in lines[i]:
        net_exists = True
        current_block = True
    if current_block and 'psk' in lines[i]:
        if not '"{}"'.format(pw) in lines[i]:
            parts = lines[i].split('=')
            lines[i] = '{}="{}"\n'.format(parts[0], pw)
    if '}' in lines[i]:
        current_block = False

if not net_exists:
    print('adding new network')
    lines.append('\n')
    lines.append('network={\n')
    lines.append('\tssid="{}"\n'.format(ssid))
    lines.append('\tpsk="{}"\n'.format(pw))
    lines.append('\tkey_mgmt=WPA-PSK\n')
    lines.append('}\n')
    out_f = open(wifi_config_file, 'w')
    for l in lines:
        out_f.write(l)
    out_f.close()

print('ok')
sys.exit(0)
