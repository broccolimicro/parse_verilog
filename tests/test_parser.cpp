#include <gtest/gtest.h>
#include <parse_verilog/module.h>
#include <parse_verilog/assign.h>
#include <parse_verilog/trigger.h>
#include <parse_verilog/if_statement.h>
#include <parse_verilog/block_statement.h>
#include <parse_verilog/assignment_statement.h>
#include <parse_verilog/declaration.h>
#include <parse_verilog/loop_statement.h>
#include <parse_verilog/module_instance.h>
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

TEST(VerilogParser, ModuleWithAssignment) {
	// Test module with continuous assignment
	string verilog_code = 
R"(module assignment_test(
	input wire [7:0] a,
	input wire [7:0] b,
	output wire [7:0] c
);
	assign c = a + b;
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("assignment_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "assignment_test");
	EXPECT_EQ(dut.ports.size(), 3u);
	EXPECT_EQ(dut.items.size(), 1u);  // One assignment
	
	// Verify the assignment
	if (dut.items.size() == 1) {
		shared_ptr<parse::syntax> item = dut.items[0];
		EXPECT_TRUE(dynamic_cast<assign*>(item.get()) != nullptr);
		
		assign* assign_stmt = dynamic_cast<assign*>(item.get());
		EXPECT_EQ(assign_stmt->name.to_string(), "c");
		EXPECT_TRUE(assign_stmt->expr.valid);
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, AlwaysBlock) {
	// Test module with always block
	string verilog_code = 
R"(module counter(
	input wire clk,
	input wire reset,
	output reg [7:0] count
);
	always @(posedge clk or posedge reset) 
	begin
		if (reset)
			count = 0;
		else
			count = count + 1;
	end
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("counter_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "counter");
	EXPECT_EQ(dut.ports.size(), 3u);
	EXPECT_EQ(dut.items.size(), 1u);  // One always block
	
	// Verify the always block
	if (dut.items.size() == 1) {
		shared_ptr<parse::syntax> item = dut.items[0];
		EXPECT_TRUE(dynamic_cast<trigger*>(item.get()) != nullptr);
		
		trigger* always_block = dynamic_cast<trigger*>(item.get());
		EXPECT_TRUE(always_block->condition.valid);
		EXPECT_TRUE(always_block->body.valid);
		EXPECT_GT(always_block->body.sub.size(), 0u);
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, ComplexPortDeclarations) {
	// Test module with complex port declarations
	string verilog_code = 
R"(module complex_ports(
	input wire clk,
	input wire [31:0] data_in,
	input wire [3:0][7:0] packet,
	output reg [15:0] result,
	inout wire [7:0] bidir
);
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("complex_ports_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "complex_ports");
	EXPECT_EQ(dut.ports.size(), 5u);
	
	// Check the more complex port declarations
	if (dut.ports.size() >= 5) {
		// Check data_in - 32-bit input
		EXPECT_EQ(dut.ports[1].name, "data_in");
		EXPECT_TRUE(dut.ports[1].input);
		EXPECT_FALSE(dut.ports[1].output);
		EXPECT_EQ(dut.ports[1].type, "wire");
		
		// Check packet - multi-dimensional input
		EXPECT_EQ(dut.ports[2].name, "packet");
		EXPECT_TRUE(dut.ports[2].input);
		EXPECT_FALSE(dut.ports[2].output);
		EXPECT_EQ(dut.ports[2].type, "wire");
		
		// Check bidirectional port
		EXPECT_EQ(dut.ports[4].name, "bidir");
		EXPECT_TRUE(dut.ports[4].input);
		EXPECT_TRUE(dut.ports[4].output);
		EXPECT_EQ(dut.ports[4].type, "wire");
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, NestedIfStatement) {
	// Test module with nested if statements
	string verilog_code = 
R"(module nested_if(
	input wire clk,
	input wire [1:0] sel,
	input wire [7:0] a,
	input wire [7:0] b,
	input wire [7:0] c,
	output reg [7:0] out
);
	always @(sel or a or b or c) 
	begin
		if (sel == 2'b00)
			out = a;
		else if (sel == 2'b01)
			out = b;
		else if (sel == 2'b10)
			out = c;
		else
			out = 8'h00;
	end
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("nested_if_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "nested_if");
	EXPECT_EQ(dut.ports.size(), 6u);
	EXPECT_EQ(dut.items.size(), 1u);  // One always block
	
	// Verify the always block with nested if statements
	if (dut.items.size() == 1) {
		shared_ptr<parse::syntax> item = dut.items[0];
		EXPECT_TRUE(dynamic_cast<trigger*>(item.get()) != nullptr);
		
		trigger* always_block = dynamic_cast<trigger*>(item.get());
		EXPECT_TRUE(always_block->condition.valid);
		EXPECT_TRUE(always_block->body.valid);
		
		// The body should contain an if statement
		if (always_block->body.sub.size() > 0) {
			shared_ptr<parse::syntax> if_stmt = always_block->body.sub[0];
			EXPECT_TRUE(dynamic_cast<if_statement*>(if_stmt.get()) != nullptr);
			
			if_statement* if_ptr = dynamic_cast<if_statement*>(if_stmt.get());
			// Check for conditions (if, else if, else)
			EXPECT_GE(if_ptr->condition.size(), 3u);
			EXPECT_GE(if_ptr->body.size(), 4u);
		}
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, MultipleAssignments) {
	// Test module with multiple continuous assignments
	string verilog_code = 
R"(module multiple_assigns(
	input wire [7:0] a,
	input wire [7:0] b,
	output wire [7:0] sum,
	output wire [7:0] diff,
	output wire [7:0] product
);
	assign sum = a + b;
	assign diff = a - b;
	assign product = a * b;
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("multiple_assigns_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "multiple_assigns");
	EXPECT_EQ(dut.ports.size(), 5u);
	EXPECT_EQ(dut.items.size(), 3u);  // Three assignments
	
	// Verify each assignment
	if (dut.items.size() == 3) {
		// First assignment (sum)
		shared_ptr<parse::syntax> item1 = dut.items[0];
		EXPECT_TRUE(dynamic_cast<assign*>(item1.get()) != nullptr);
		EXPECT_EQ(dynamic_cast<assign*>(item1.get())->name.to_string(), "sum");
		
		// Second assignment (diff)
		shared_ptr<parse::syntax> item2 = dut.items[1];
		EXPECT_TRUE(dynamic_cast<assign*>(item2.get()) != nullptr);
		EXPECT_EQ(dynamic_cast<assign*>(item2.get())->name.to_string(), "diff");
		
		// Third assignment (product)
		shared_ptr<parse::syntax> item3 = dut.items[2];
		EXPECT_TRUE(dynamic_cast<assign*>(item3.get()) != nullptr);
		EXPECT_EQ(dynamic_cast<assign*>(item3.get())->name.to_string(), "product");
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, ForLoop) {
	// Test module with a for loop
	string verilog_code = 
R"(module loop_statement_test(
	input wire clk,
	input wire reset,
	output reg [7:0] out
);
	always @(posedge clk) 
	begin
		for (i = 0; i < 8; i = i + 1)
		begin
			out[i] = ~out[i];
		end
	end
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("loop_statement_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "loop_statement_test");
	EXPECT_EQ(dut.ports.size(), 3u);
	EXPECT_EQ(dut.items.size(), 1u);  // One always block with for loop
	
	// Verify the always block with for loop
	if (dut.items.size() == 1) {
		shared_ptr<parse::syntax> item = dut.items[0];
		EXPECT_TRUE(dynamic_cast<trigger*>(item.get()) != nullptr);
		
		trigger* always_block = dynamic_cast<trigger*>(item.get());
		EXPECT_TRUE(always_block->condition.valid);
		EXPECT_TRUE(always_block->body.valid);
		
		// The body should contain a for loop
		if (always_block->body.sub.size() > 0) {
			shared_ptr<parse::syntax> loop = always_block->body.sub[0];
			EXPECT_TRUE(dynamic_cast<loop_statement*>(loop.get()) != nullptr);
			
			loop_statement* for_ptr = dynamic_cast<loop_statement*>(loop.get());
			// Check for loop components - updated to match new implementation
			EXPECT_TRUE(for_ptr->init.valid);
			EXPECT_TRUE(for_ptr->cond.valid);
			EXPECT_TRUE(for_ptr->step.valid);
			EXPECT_TRUE(for_ptr->body.valid);
			
			// Check that initialization is "i = 0"
			EXPECT_EQ(for_ptr->init.name.to_string(), "i");
			
			// Check that step is "i = i + 1"
			EXPECT_EQ(for_ptr->step.name.to_string(), "i");
		}
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, ModuleInstantiation) {
	// Test module with module instantiation
	string verilog_code = 
R"(module top_module(
	input wire clk,
	input wire reset,
	output wire [7:0] out
);
	counter counter_inst(
		.clk(clk),
		.reset(reset),
		.count(out)
	);
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("module_inst_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "top_module");
	EXPECT_EQ(dut.ports.size(), 3u);
	EXPECT_EQ(dut.items.size(), 1u);  // One module instantiation
	
	// Verify the module instantiation
	if (dut.items.size() == 1) {
		shared_ptr<parse::syntax> item = dut.items[0];
		EXPECT_TRUE(dynamic_cast<module_instance*>(item.get()) != nullptr);
		
		module_instance* inst_ptr = dynamic_cast<module_instance*>(item.get());
		// Check module instance details
		EXPECT_EQ(inst_ptr->module_type, "counter");
		EXPECT_EQ(inst_ptr->instance_name, "counter_inst");
		EXPECT_EQ(inst_ptr->connections.size(), 3u);
		
		// Check port connections
		if (inst_ptr->connections.size() >= 3) {
			EXPECT_EQ(inst_ptr->connections[0].port_name, "clk");
			EXPECT_TRUE(inst_ptr->connections[0].expr.valid);
			
			EXPECT_EQ(inst_ptr->connections[1].port_name, "reset");
			EXPECT_TRUE(inst_ptr->connections[1].expr.valid);
			
			EXPECT_EQ(inst_ptr->connections[2].port_name, "count");
			EXPECT_TRUE(inst_ptr->connections[2].expr.valid);
		}
	}

	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, MixedItemTypes) {
	// Test module with various item types
	string verilog_code = 
R"(module mixed_items(
	input wire clk,
	input wire reset,
	input wire [7:0] data_in,
	output reg [7:0] data_out
);
	wire [7:0] intermediate;
	
	// Continuous assignment
	assign intermediate = data_in + 8'h01;
	
	// Module instantiation
	adder adder_inst(
		.a(intermediate),
		.b(8'h02),
		.sum(data_out)
	);
	
	// Always block with for loop
	always @(posedge clk) 
	begin
		for (i = 0; i < 8; i = i + 1)
		begin
			if (reset)
				data_out[i] = 0;
		end
	end
endmodule)";
	
	tokenizer tokens;
	module_def::register_syntax(tokens);
	tokens.insert("mixed_items_test", verilog_code);
	
	module_def dut(tokens);
	EXPECT_TRUE(dut.valid);
	EXPECT_EQ(dut.name, "mixed_items");
	EXPECT_EQ(dut.ports.size(), 4u);
	
	// Verify the mix of item types (one wire declaration, one assign, one module instance, one always block)
	EXPECT_GE(dut.items.size(), 3u);
	
	// Check overall string representation
	EXPECT_EQ(dut.to_string(""), verilog_code);
}

TEST(VerilogParser, ParseAndRegenerate) {
	// Test that parsing and regenerating code produces the original code
	// This is a comprehensive test for the to_string functionality
	vector<string> test_cases = {
		// Simple module
		R"(module simple();
endmodule)",
		
		// Module with ports
		R"(module with_ports(
	input a,
	output b
);
endmodule)",
		
		// Module with continuous assignment
		R"(module with_assign(
	input in,
	output out
);
	assign out = in;
endmodule)",
		
		// Module with always block
		R"(module with_always(
	input clk,
	output reg q
);
	always @(posedge clk)
	begin
		q = 1;
	end
endmodule)",

		// Module with for loop
		R"(module with_for(
	input clk
);
	always @(posedge clk)
	begin
		for (i = 0; i < 10; i = i + 1)
		begin
			$display(i);
		end
	end
endmodule)",

		// Module with module instantiation
		R"(module with_inst(
	input in,
	output out
);
	buffer buf1(
		.in(in),
		.out(out)
	);
endmodule)"
	};
	
	for (const auto& test_case : test_cases) {
		tokenizer tokens;
		module_def::register_syntax(tokens);
		tokens.insert("test_case", test_case);
		
		module_def dut(tokens);
		EXPECT_TRUE(dut.valid);
		EXPECT_EQ(dut.to_string(""), test_case);
	}
}

