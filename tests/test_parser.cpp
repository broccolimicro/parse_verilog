#include <gtest/gtest.h>
#include <parse_verilog/module.h>
#include <sstream>
#include <string>

using namespace std;
using namespace parse_verilog;

TEST(VerilogParser, BasicModule) {
	// Sample Verilog code
	string verilog_code = 
R"(module test_module(
	input wire clk,
	input wire reset,
	output reg [7:0] data_out
);
endmodule)";
	
	// Set up tokenizer
	tokenizer tokens;
	module_def::register_syntax(tokens);
	
	// Load the Verilog code
	istringstream stream(verilog_code);
	tokens.insert("test_code", verilog_code);
	
	// Parse the module
	parse_verilog::module_def dut(tokens);
	
	// Check if parsing was successful
	EXPECT_TRUE(dut.valid);
	
	// Check module name
	EXPECT_EQ(dut.name, "test_module");
	
	// Check port count
	EXPECT_EQ(dut.ports.size(), 3u);
	
	// Check port details
	if (dut.ports.size() >= 3) {
		// Check first port (clk)
		EXPECT_EQ(dut.ports[0].name, "clk");
		EXPECT_TRUE(dut.ports[0].input);
		EXPECT_FALSE(dut.ports[0].output);
		EXPECT_EQ(dut.ports[0].type, "wire");
		EXPECT_TRUE(dut.ports[0].size.empty());
		
		// Check second port (reset)
		EXPECT_EQ(dut.ports[1].name, "reset");
		EXPECT_TRUE(dut.ports[1].input);
		EXPECT_FALSE(dut.ports[1].output);
		EXPECT_EQ(dut.ports[1].type, "wire");
		EXPECT_TRUE(dut.ports[1].size.empty());
		
		// Check third port (data_out)
		EXPECT_EQ(dut.ports[2].name, "data_out");
		EXPECT_FALSE(dut.ports[2].input);
		EXPECT_TRUE(dut.ports[2].output);
		EXPECT_EQ(dut.ports[2].type, "reg");
		EXPECT_TRUE(dut.ports[2].size.empty());
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

