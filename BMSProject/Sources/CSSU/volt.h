#ifndef TASK_VOLT_PROCESS_
#define TASK_VOLT_PROCESS_

  #define NUM_IC                 3

  #define NUM1_Batper            9
  #define NUM2_Batper            8  
  #define NUM3_Batper            8
  #define NUM4_Batper            0
  #define NUM5_Batper            0

  #define NUM1_Batper_true       9
  #define NUM1_Batper_front      5
  #define NUM1_Batper_rear       4
  #define NUM1_Batper_empty      1

  #define NUM2_Batper_true       8
  #define NUM2_Batper_front      4
  #define NUM2_Batper_rear       4
  #define NUM2_Batper_empty      2

  #define NUM3_Batper_true       8
  #define NUM3_Batper_front      4
  #define NUM3_Batper_rear       4
  #define NUM3_Batper_empty      2

  #define NUM4_Batper_true       0
  #define NUM4_Batper_front      0
  #define NUM4_Batper_rear       0
  #define NUM4_Batper_empty      0

  #define NUM5_Batper_true       0
  #define NUM5_Batper_front      0
  #define NUM5_Batper_rear       0
  #define NUM5_Batper_empty      0

  #define NUM_Battery      (NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)
  //#define NUM_Battery      NUM_IC*NUM_Batper
  #define NUM_pack               0   //子板的编号(决定子板的ID号)

  #define NUM_Tem                5
  #define Cell_GPIO              2

#endif