#include <gtest/gtest.h>
#include <parse_verilog/parse_verilog.h>
#include <sstream>

TEST(VerilogParser, BasicModule) {
    // Sample Verilog code
    std::string verilog_code = 
        "module test_module(\n"
        "    input wire clk,\n"
        "    input wire reset,\n"
        "    output reg [7:0] data_out\n"
        ");\n"
        "endmodule\n";
    
    // Set up tokenizer
    tokenizer tokens;
    parse_verilog::register_syntax(tokens);
    
    // Load the Verilog code
    std::istringstream stream(verilog_code);
    tokens.insert("test_code", verilog_code);
    
    // Parse the module
    parse_verilog::module* module = (parse_verilog::module*)parse_verilog::produce(tokens);
    
    // Check if parsing was successful
    EXPECT_TRUE(module->valid);
    
    // Check module name
    EXPECT_EQ(module->name, "test_module");
    
    // Check port count
    EXPECT_EQ(module->ports.size(), 3);
    
    // Check port details
    if (module->ports.size() >= 3) {
        // Check first port (clk)
        EXPECT_EQ(module->ports[0]->name, "clk");
        EXPECT_EQ(module->ports[0]->direction, parse_verilog::INPUT);
        EXPECT_EQ(module->ports[0]->type, parse_verilog::WIRE);
        EXPECT_FALSE(module->ports[0]->is_vector);
        
        // Check second port (reset)
        EXPECT_EQ(module->ports[1]->name, "reset");
        EXPECT_EQ(module->ports[1]->direction, parse_verilog::INPUT);
        EXPECT_EQ(module->ports[1]->type, parse_verilog::WIRE);
        EXPECT_FALSE(module->ports[1]->is_vector);
        
        // Check third port (data_out)
        EXPECT_EQ(module->ports[2]->name, "data_out");
        EXPECT_EQ(module->ports[2]->direction, parse_verilog::OUTPUT);
        EXPECT_EQ(module->ports[2]->type, parse_verilog::REG);
        EXPECT_TRUE(module->ports[2]->is_vector);
    }
    
    delete module;
}

TEST(VerilogParser, ModuleWithParameters) {
    // Sample Verilog code with parameters
    std::string verilog_code = 
        "module counter #(\n"
        "    parameter WIDTH = 8,\n"
        "    parameter MAX_COUNT = 255\n"
        ")(\n"
        "    input wire clk,\n"
        "    input wire reset,\n"
        "    output reg [WIDTH-1:0] count\n"
        ");\n"
        "endmodule\n";
    
    // Set up tokenizer
    tokenizer tokens;
    parse_verilog::register_syntax(tokens);
    
    // Load the Verilog code
    tokens.insert("test_code", verilog_code);
    
    // Parse the module
    parse_verilog::module* module = (parse_verilog::module*)parse_verilog::produce(tokens);
    
    // Check if parsing was successful
    EXPECT_TRUE(module->valid);
    
    // Check module name
    EXPECT_EQ(module->name, "counter");
    
    // Check parameter count
    EXPECT_EQ(module->parameters.size(), 2);
    
    // Check parameter details
    if (module->parameters.size() >= 2) {
        EXPECT_EQ(module->parameters[0]->name, "WIDTH");
        EXPECT_EQ(module->parameters[1]->name, "MAX_COUNT");
    }
    
    // Check port count
    EXPECT_EQ(module->ports.size(), 3);
    
    delete module;
} 