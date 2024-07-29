`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 
// 	Dr. Eros Camacho Ruiz
//	IMSE
//
//////////////////////////////////////////////////////////////////////////////////


module tb_keccak();

    parameter period = 20;
    
    parameter SHA = 256; // 224, 256, 384, 512
    parameter BLOCK_LENGTH = 1600 - (2*SHA);
    parameter SHAKE = 1;
    parameter OUTPUT_LENGTH = SHA;  
    parameter OUTPUT_LENGTH_CEIL = ceil(OUTPUT_LENGTH,64);
    
    
    reg                                 clk;
    reg                                 reset;
    reg                                 rst;
    reg                                 load_padding;
    reg                                 start_padding;
    reg                                 start;
    reg                                 load;
    reg                                 read;  
    reg                                 start_padding_double;  
    reg                                 start_shake;  
    wire    [7:0]                       control;
    reg     [63:0]                      data_in;
    reg     [4:0]                       add;
    wire                                fin;
    wire    [1599:0]                    P;
    wire    [0:OUTPUT_LENGTH - 1]       P_o;
    wire    [0:BLOCK_LENGTH - 1]        P_o2;
    wire    [1599:0]                    S_theta;
    wire    [7:0]                       round;
    wire    [63:0]                      data_out;
    wire                                end_op;
    wire    [63:0]                      m_wire;
    
    wire    [63:0]                      probe;
    
    
    assign control = {start_shake,start_padding_double, read,start,load,start_padding,load_padding,rst};
    
    integer j;
    
    parameter STAGES = 1;
    parameter D_WIDTH = 64;
    
    //keccak_stages #(.STAGES(STAGES)) keccak (.clk(clk), .rst(rst), .S(P), .S_o(P_o), .ini(ini), .fin(fin)); 
    
    sha3_xl #(.BLOCK_LENGTH(BLOCK_LENGTH), .SHAKE(SHAKE),
.OUTPUT_LENGTH(OUTPUT_LENGTH),.STAGES(STAGES), .D_WIDTH(D_WIDTH)) 
    sha3_xl  (.i_clk(clk), .i_reset(reset), .i_data_in(data_in), .i_add(add), .i_control(control), .o_data_out(data_out), .o_end_op(end_op));
    
    //keccak_ctl  keccak_ctl  (.clk(clk), .rst(rst),  .ini(ini),  .fin(fin),  .round(round));
    //theta   theta   (.S(P),         .S_o(S_theta));
    //rho_pi  rho_pi  (.S(P),   .S_o(S_theta));
    //chi     chi     (.S(P),  .S_o(S_theta));
    //keccak_au   keccak_au   (.clk(clk), .rst(rst),  .S(P), .S_o(S_theta),  .round(0));
    
    wire [63:0]     S1       [0:(BLOCK_LENGTH/64) - 1];
    reg  [63:0]     S2       [0:(BLOCK_LENGTH/64) - 1];
    reg  [63:0]     So       [0:(BLOCK_LENGTH/64) - 1];
    
    genvar i;

    /*for ( i = 0; i < 25; i = i + 1) begin
        assign S1[i] = S_theta[((i+1)*64-1):i*64];
    end*/
    /*
    for ( i = 0; i < 8; i = i + 1) begin
        assign S1[i] = P_o[((i+1)*64-1):i*64];
    end
    endgenerate
    
    generate 
    for ( i = 0; i < 25; i = i + 1) begin
        assign P[((i+1)*64-1):i*64] = S2[i];
    end
    endgenerate
    */

    generate 
    if(SHAKE == 0) begin
        
        reg  [63:0]     So       [0:OUTPUT_LENGTH_CEIL - 1];
        
        for ( i = 0; i < OUTPUT_LENGTH_CEIL; i = i + 1) begin
            if(i == 3 & SHA == 224) assign P_o[192:223] = So[i][63:32];
            else                    assign P_o[i*64:((i+1)*64-1)] = So[i];
        end
    end
    else begin
        
        reg  [63:0]     So       [0:(BLOCK_LENGTH/64) - 1];
        
        for ( i = 0; i < (BLOCK_LENGTH/64); i = i + 1) begin    
            if(i == 3 & SHA == 224) assign P_o[192:223] = So[i][63:32];
            else                    assign P_o[i*64:((i+1)*64-1)] = So[i];
        end
    end
    endgenerate
    
    generate 
    for ( i = 0; i < (BLOCK_LENGTH/64); i = i + 1) begin
       assign S1[i] = P_o2[i*64:((i+1)*64-1)];
    end
    endgenerate
    
    
    
    initial begin
        
        for ( j = 0; j < 25; j = j + 1) begin
            S2[j] = 0;
            So[j] = 0;
        end
        //S2[0]  = 64'h00000000_0000001F;
        //S2[16] = 64'h80000000_00000000;
        
        //S2[0] = 64'h8d8001e2_c096f1b8;
        //S2[0] = 64'hb8f196c0_e201808d;
        //S2[1] = 64'h8e7c9224_a086efd4;
        //S2[1] = 64'hd4ef86a0_24927c8e;
        //S2[2] = 64'h797fbf74_a8033a2d;
        //S2[2] = 64'h2d3a03a8_74bf7f79;
        //S2[3] = 64'h422a2b6b_8f6747e4;
        //S2[3] = 64'he447678f_6b2b2a42;
        
        
        //S2[0] = 64'hef896cdc_b363a615;
        //S2[0] = 64'h15a663b3_dc6c89ef;
        //S2[1] = 64'h9178a1bb_1c993946;
        //S2[1] = 64'h4639991c_bba17891;
        //S2[2] = 64'hc5040209_5cdaea4f;
        //S2[2] = 64'h4feada5c_090204c5;
        //S2[3] = 64'hd4d419aa_47321c88;
        //S2[3] = 64'h881c3247_aa19d4d4;
        //S2[1] = 64'h87b375f8_9bfee549;
        //S2[2] = 64'hd49ffacc_462d3257;
        //S2[3] = 64'hcc05e251_e2fec0b6;
        //S2[4] = 64'h6cb866f3_cb7bb6da;
        //S2[5] = 64'hd585c473_4066ff48;
        //S2[6] = 64'hcfa8bc66_e6b1c81d;
        //S2[7] = 64'h415412b6_43070587;
        //S2[8] = 64'h1b2f0e20_4441ec4b;
        
        S2[0]   = 64'hae471df8972bf1d8;
        S2[1]   = 64'h2c1704ff1473fbbb;
        S2[2]   = 64'h8b238e77c371bef2;
        S2[3]   = 64'h54bd1f69cbeecdcc;
        S2[4]   = 64'hb5aba0b1b7e5002b;
        S2[5]   = 64'h00a7fe81f707f107;
        S2[6]   = 64'h29e0a9de5f377eea;
        S2[7]   = 64'h4b771622a60e4a75;
        S2[8]   = 64'h23023d78e8593cda;
        S2[9]   = 64'h930d1c622596fe60;
        S2[10]  = 64'h21943236c07b7fe2;
        S2[11]  = 64'h758a049d016f7150;
        S2[12]  = 64'hdf559c13930fcc2c;
        S2[13]  = 64'hcf50056a06aa4a0f;
        S2[14]  = 64'h5b47d0474ec5e822;
        S2[15]  = 64'hbcff92a342986ba5;
        S2[16]  = 64'hd1ab644b1e8fd96b;
        
        // Reset
        reset = 0; rst = 1; load_padding = 0; start_padding = 0; start_padding_double = 0; load = 0; start = 0; start_shake = 0; read = 0; data_in = 0;        add = 0; #(2*period);
        reset = 1; rst = 1; load_padding = 0; start_padding = 0; start_padding_double = 0; load = 0; start = 0; start_shake = 0; read = 0; data_in = 0;        add = 0; #(2*period);
        
        // Load Padding
         reset = 1; rst = 0; load_padding = 0; start_padding = 0; start_padding_double = 0; load = 0; start = 0; start_shake = 0; read = 0; data_in = 0; #(period); add = 0; #(period);
        
        // Input data
        for ( j = 0; j < (BLOCK_LENGTH / 64); j = j + 1) begin
            reset = 1; rst = 0; load_padding = 0; start_padding = 0; start_padding_double = 0; load = 1; start = 0; start_shake = 0; read = 0; add = j; #(period);  data_in = S2[j]; #(period);
        end
        
        // Start
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 1; start_shake = 0; read = 0; data_in = S2[8];    add = 8; #(period); // Si pongo lo mismo que add = 8 no hay problema.
        while(!end_op) #(period);
         #(10*period);
         
         // Load Padding
         reset = 1; rst = 0; load_padding = 1; start_padding = 0; start_padding_double = 0; load = 0; start = 0; start_shake = 0; read = 0; data_in = 0; #(period); add = 0; #(period);
        
        // Input data
        for ( j = 0; j < (BLOCK_LENGTH / 64); j = j + 1) begin
            reset = 1; rst = 0; load_padding = 0; start_padding = 1; start_padding_double = 0; load = 1; start = 0; start_shake = 0; read = 0; add = j; #(period);  data_in = 0; #(period);
        end
        
         
        // Read Data
        if(SHAKE == 0) begin
            for ( j = 0; j < OUTPUT_LENGTH_CEIL; j = j + 1) begin
                reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = j;  #(period); So[j] = SHA3_H2B64(data_out);  #(period);
            end
        end
        else begin
            for ( j = 0; j < (1600/64); j = j + 1) begin
            reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = j;  #(period); So[j] = SHA3_H2B64(data_out);  #(period);
            end
        end
         
        // Start SHAKE
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; start_shake = 1; read = 0; data_in = S2[8];    add = 8; #(period); // Si pongo lo mismo que add = 8 no hay problema.
        // Start
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 1; start_shake = 0; read = 0; data_in = S2[8];    add = 8; #(period); // Si pongo lo mismo que add = 8 no hay problema.
        while(!end_op) #(period);
         #(10*period);
         
         // Read Data
        if(SHAKE == 0) begin
            for ( j = 0; j < OUTPUT_LENGTH_CEIL; j = j + 1) begin
                reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = j;  #(period); So[j] = SHA3_H2B64(data_out);  #(period);
            end
        end
        else begin
            for ( j = 0; j < (1600/64); j = j + 1) begin
            reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = j;  #(period); So[j] = SHA3_H2B64(data_out);  #(period);
            end
        end
        /* 
        for ( j = 0; j < 25; j = j + 1) begin
            S2[j] = 0;
            //So[j] = 0;
        end
        
        // Input data
        for ( j = 0; j < (BLOCK_LENGTH / 64); j = j + 1) begin
            reset = 1; rst = 0; load_padding = 0; start_padding = 1; start_padding_double = 0; load = 1; start = 0; read = 0; add = j; #(period); data_in = S2[j]; #(period);
        end
        
        // Start
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 1; read = 0; data_in = S2[8];    add = 8; #(period); // Si pongo lo mismo que add = 8 no hay problema.
        while(!end_op) #(period);
         #(10*period);
         */
        // Read Data
        for ( j = 0; j < OUTPUT_LENGTH_CEIL; j = j + 1) begin
            reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = j;  #(period); So[j] = SHA3_H2B64(data_out);  #(period);
        end
      
        
        // MD_512 = a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26
        // MD_384 = 0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac3713831264adb47fb6bd1e058d5f004
        // MD_256 = a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a
        // MD_224 = 6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7
        
        
        
        
        
        
        
        //S2[0]   = 64'h00000000_000006e5;
        //S2[0] = {32'h00000000,32'b00000000_00000000_00000000_00001101}; // bit-aligned // 01 -> Delimited 1 -> Padding init
        //S2[8]   = 64'h80000000_00000000;
        //S2[8] = {32'b10000000_00000000_00000000_00000000, 32'h00000000};
       // S2[9] = 64'h00000000_00000001;
        /*
        for ( j = 0; j < 25; j = j + 1) begin
            S2[j] = 64'ha3000000_000000a3;
        end
        */
        
        /*
        S2[0] = 64'h68c290a9_c3f8e90c;
        S2[1] = 64'h7c0fdbef_9bfd4ef3;
        S2[2] = 64'h7111b0fd_6c46f84e;
        S2[3] = 64'h2aa9ef5f_dc70def8;
        S2[4] = 64'h5c1aace2_293de9cb;
        S2[5] = 64'h0e8cb01a_9f127929;
        S2[6] = 64'h208af6dd_2479de77;
        S2[7] = 64'hc1852fc6_ada0df9c;
        S2[8] = 64'hf84f3fb3_c6d93786;
        
        reset = 0; rst = 1; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 0; data_in = 0;        add = 0; #(2*period);
        reset = 1; rst = 1; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 0; data_in = 0;        add = 0; #(2*period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[0];    add = 0; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[1];    add = 1; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[2];    add = 2; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[3];    add = 3; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[4];    add = 4; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[5];    add = 5; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[6];    add = 6; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[7];    add = 7; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[8];    add = 8; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 1; read = 0; data_in = S2[8];    add = 8; #(period); // Si pongo lo mismo que add = 8 no hay problema.
        
        while(!end_op) #(period);
        
        #(10*period);
        
        
        //rst = 0; ini = 0; #(4*period);
        //rst = 1; ini = 0; #period;
        //rst = 1; ini = 1; #period;
        //rst = 1; ini = 0; #(30*period);
        
        
        //S2[0] = 64'h00000000_00000006;
        S2[0] = 64'h00000000_00000000;
        S2[1] = 64'h00000000_00000000;
        S2[2] = 64'h00000000_00000000;
        S2[3] = 64'h00000000_00000000;
        S2[4] = 64'h00000000_00000000;
        S2[5] = 64'h00000000_00000000;
        S2[6] = 64'h00000000_00000000;
        S2[7] = 64'h00000000_00000000;
        S2[8] = 64'h00000000_00000000;
        //S2[8] = 64'h80000000_00000000;
        
        reset = 1; rst = 0; load_padding = 1; start_padding = 0; load = 0; start = 0; read = 0; data_in = 0;        add = 0; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; #(10*period); data_in = S2[0];    add = 0; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; #(10*period); data_in = S2[1];    add = 1; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[2];    add = 2; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[3];    add = 3; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[4];    add = 4; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[5];    add = 5; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[6];    add = 6; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 1; start = 0; read = 0; data_in = S2[7];    add = 7; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; #(10*period); data_in = S2[8];    add = 8; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 1; read = 0; data_in = S2[8];    add = 8; #(period);
        
        while(!end_op) #(period);
        
        #(10*period);
        
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 0;  #(period/2); So[0] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 1;  #(period/2); So[1] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 2;  #(period/2); So[2] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 3;  #(period/2); So[3] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 4;  #(period/2); So[4] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 5;  #(period/2); So[5] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 6;  #(period/2); So[6] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 7;  #(period/2); So[7] = SHA3_H2B64(data_out);  #(period/2);
        
        //rst = 1; ini = 1; #(period);
        //rst = 1; ini = 0;
       
        */
        
        /*
        S2[0] = 64'h6b1477ab_c7fff5e2;
        S2[1] = 64'h00000000_00000000;
        S2[2] = 64'h00000000_00000000;
        S2[3] = 64'h00000000_00000000;
        S2[4] = 64'h00000000_00000000;
        S2[5] = 64'h00000000_00000000;
        S2[6] = 64'h00000000_00000000;
        S2[7] = 64'h00000000_00000000;
        S2[8] = 64'h00000000_00000000;
        
        reset = 0; rst = 1; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 0; data_in = 0;                            add = 0; #(2*period);
        reset = 1; rst = 1; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 0; data_in = 0;                            add = 0; #(2*period);
        reset = 1; rst = 0; load_padding = 1; start_padding = 0; load = 0; start = 0; read = 0; data_in = 64'h00000000_0000003F;        add = 0; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[0];                        add = 0; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[1];                        add = 1; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[2];                        add = 2; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[3];                        add = 3; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[4];                        add = 4; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[5];                        add = 5; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[6];                        add = 6; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[7];                        add = 7; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 1; load = 1; start = 0; read = 0; data_in = S2[8];                        add = 8; #(period);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 1; read = 0; data_in = S2[8];                        add = 8; #(period);
        
        while(!end_op) #(period);
        
        #(10*period);
        
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 0;  #(period/2); So[0] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 1;  #(period/2); So[1] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 2;  #(period/2); So[2] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 3;  #(period/2); So[3] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 4;  #(period/2); So[4] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 5;  #(period/2); So[5] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 6;  #(period/2); So[6] = SHA3_H2B64(data_out);  #(period/2);
        reset = 1; rst = 0; load_padding = 0; start_padding = 0; load = 0; start = 0; read = 1; add = 7;  #(period/2); So[7] = SHA3_H2B64(data_out);  #(period/2);
        */
    end
    
    // Generacion de la se al de reloj
    initial begin
        clk = 0; 
        forever begin
        #(period/2) clk = ~clk;
        end
    end
    
    function [63:0] SHA3_H2B64 (input [63:0] x);
    begin
	SHA3_H2B64 = {x[7:0],x[15:8],x[23:16],x[31:24],x[39:32],x[47:40],x[55:48],x[63:56]};
	end
    endfunction
    
    // ceil function 
    function integer ceil;
      input integer n;
      input integer b;
        for (ceil=0; n>0; ceil=ceil+1)
          n = n - b;
    endfunction
    
endmodule
