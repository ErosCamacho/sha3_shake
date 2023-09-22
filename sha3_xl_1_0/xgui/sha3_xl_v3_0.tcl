
# Loading additional proc with user specified bodies to compute parameter values.
source [file join [file dirname [file dirname [info script]]] gui/sha3_xl_v2_0.gtcl]

# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  ipgui::add_param $IPINST -name "VERSION" -widget comboBox
  ipgui::add_param $IPINST -name "BLOCK_LENGTH"
  ipgui::add_param $IPINST -name "OUTPUT_LENGTH"
  ipgui::add_static_text $IPINST -name "SPACE" -text { 

}
  set STAGES [ipgui::add_param $IPINST -name "STAGES" -widget comboBox]
  set_property tooltip {STAGES} ${STAGES}
  set SHAKE [ipgui::add_param $IPINST -name "SHAKE"]
  set_property tooltip {SHAKE} ${SHAKE}
  ipgui::add_param $IPINST -name "STAGES_REG"

}

proc update_PARAM_VALUE.BLOCK_LENGTH { PARAM_VALUE.BLOCK_LENGTH PARAM_VALUE.OUTPUT_LENGTH } {
	# Procedure called to update BLOCK_LENGTH when any of the dependent parameters in the arguments change
	
	set BLOCK_LENGTH ${PARAM_VALUE.BLOCK_LENGTH}
	set OUTPUT_LENGTH ${PARAM_VALUE.OUTPUT_LENGTH}
	set values(OUTPUT_LENGTH) [get_property value $OUTPUT_LENGTH]
	set_property value [gen_USERPARAMETER_BLOCK_LENGTH_VALUE $values(OUTPUT_LENGTH)] $BLOCK_LENGTH
}

proc validate_PARAM_VALUE.BLOCK_LENGTH { PARAM_VALUE.BLOCK_LENGTH } {
	# Procedure called to validate BLOCK_LENGTH
	return true
}

proc update_PARAM_VALUE.OUTPUT_LENGTH { PARAM_VALUE.OUTPUT_LENGTH PARAM_VALUE.VERSION } {
	# Procedure called to update OUTPUT_LENGTH when any of the dependent parameters in the arguments change
	
	set OUTPUT_LENGTH ${PARAM_VALUE.OUTPUT_LENGTH}
	set VERSION ${PARAM_VALUE.VERSION}
	set values(VERSION) [get_property value $VERSION]
	set_property value [gen_USERPARAMETER_OUTPUT_LENGTH_VALUE $values(VERSION)] $OUTPUT_LENGTH
}

proc validate_PARAM_VALUE.OUTPUT_LENGTH { PARAM_VALUE.OUTPUT_LENGTH } {
	# Procedure called to validate OUTPUT_LENGTH
	return true
}

proc update_PARAM_VALUE.SHAKE { PARAM_VALUE.SHAKE PARAM_VALUE.VERSION } {
	# Procedure called to update SHAKE when any of the dependent parameters in the arguments change
	
	set SHAKE ${PARAM_VALUE.SHAKE}
	set VERSION ${PARAM_VALUE.VERSION}
	set values(VERSION) [get_property value $VERSION]
	set_property value [gen_USERPARAMETER_SHAKE_VALUE $values(VERSION)] $SHAKE
}

proc validate_PARAM_VALUE.SHAKE { PARAM_VALUE.SHAKE } {
	# Procedure called to validate SHAKE
	return true
}

proc update_PARAM_VALUE.STAGES { PARAM_VALUE.STAGES } {
	# Procedure called to update STAGES when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.STAGES { PARAM_VALUE.STAGES } {
	# Procedure called to validate STAGES
	return true
}

proc update_PARAM_VALUE.STAGES_REG { PARAM_VALUE.STAGES_REG } {
	# Procedure called to update STAGES_REG when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.STAGES_REG { PARAM_VALUE.STAGES_REG } {
	# Procedure called to validate STAGES_REG
	return true
}

proc update_PARAM_VALUE.VERSION { PARAM_VALUE.VERSION } {
	# Procedure called to update VERSION when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.VERSION { PARAM_VALUE.VERSION } {
	# Procedure called to validate VERSION
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.OUTPUT_LENGTH { MODELPARAM_VALUE.OUTPUT_LENGTH PARAM_VALUE.OUTPUT_LENGTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.OUTPUT_LENGTH}] ${MODELPARAM_VALUE.OUTPUT_LENGTH}
}

proc update_MODELPARAM_VALUE.BLOCK_LENGTH { MODELPARAM_VALUE.BLOCK_LENGTH PARAM_VALUE.BLOCK_LENGTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BLOCK_LENGTH}] ${MODELPARAM_VALUE.BLOCK_LENGTH}
}

proc update_MODELPARAM_VALUE.STAGES { MODELPARAM_VALUE.STAGES PARAM_VALUE.STAGES } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.STAGES}] ${MODELPARAM_VALUE.STAGES}
}

proc update_MODELPARAM_VALUE.SHAKE { MODELPARAM_VALUE.SHAKE PARAM_VALUE.SHAKE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SHAKE}] ${MODELPARAM_VALUE.SHAKE}
}

proc update_MODELPARAM_VALUE.STAGES_REG { MODELPARAM_VALUE.STAGES_REG PARAM_VALUE.STAGES_REG } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.STAGES_REG}] ${MODELPARAM_VALUE.STAGES_REG}
}

