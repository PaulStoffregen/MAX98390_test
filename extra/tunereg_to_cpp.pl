#! /usr/bin/perl

printf "#include <Arduino.h>\n";
while (<>) {
	if (/^\[Device Address\]/) {
		$state = 'addr';
		next;
	} elsif (/^\[Control Registers\]/) {
		$state = 'data';
		printf "PROGMEM const uint16_t MAX98390_Registers_Addr%02X[] = {\n", $i2c;
		next;
	} elsif ($state eq 'addr') {
 		if (/^0x([0-9A-Fa-f]+)/) {
			$i2c = hex($1) / 2;
		}
		$state = undef;
		next;
	} elsif ($state eq 'data') {
		if (/^0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)/) {
			($addr, $data) = (hex($1), hex($2));
			printf "\t0x%04X, 0x%02X,\n", $addr, $data;
			next;
		}
	}
	if ($state eq 'data') { 
		printf "};\n";
		$data = undef;
	}
}
if ($state eq 'data') { 
	printf "};\n";
	$data = undef;
}

