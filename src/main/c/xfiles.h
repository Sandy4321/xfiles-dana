// See LICENSE for license details.

#ifndef SRC_MAIN_C_XFILES_H_
#define SRC_MAIN_C_XFILES_H_

#include <stdint.h>
#include <stddef.h>

// [TODO] Any changes to these types need to occur in conjunction with
// the Chisel code and with the TID extraction part of
// new_write_request.
typedef int32_t nnid_type;
typedef int16_t tid_type;
typedef int32_t element_type;
typedef uint64_t xlen_t;

typedef enum {
  xfiles_reg_batch_items = 0,
  xfiles_reg_learning_rate,
  xfiles_reg_weight_decay_lambda
} xfiles_reg;

typedef enum {
  t_USR_READ_DATA = 4,
  t_USR_WRITE_DATA = 5,
  t_USR_NEW_REQUEST = 6,
  t_USR_WRITE_DATA_LAST = 7,
  t_USR_WRITE_REGISTER = 8,
  t_USR_XFILES_DEBUG = 9,
  t_USR_XFILES_DANA_ID = 10
} request_t;

typedef enum {
  FEEDFORWARD = 0,
  TRAIN_INCREMENTAL = 1,
  TRAIN_BATCH = 2
} learning_type_t;

typedef enum {
  err_XFILES_UNKNOWN = 0,
  err_XFILES_NOASID,
  err_XFILES_TTABLEFULL,
  err_XFILES_INVALIDTID
} xfiles_err_t;

typedef enum {
  resp_OK = 0,
  resp_TID,
  resp_READ,
  resp_NOT_DONE,
  resp_QUEUE_ERR,
  resp_XFILES
} xfiles_resp_t;

typedef enum {
  err_UNKNOWN     = 0,
  err_DANA_NOANTP = 1,
  err_INVASID     = 2,
  err_INVNNID     = 3,
  err_ZEROSIZE    = 4,
  err_INVEPB      = 5
} dana_err_t;

typedef enum {
  int_INVREQ      = 0,
  int_DANA_NOANTP = 1,
  int_INVASID     = 2,
  int_INVNNID     = 3,
  int_NULLREAD    = 4,
  int_ZEROSIZE    = 5,
  int_INVEPB      = 6,
  int_MISALIGNED  = 7,
  int_UNKNOWN     = -1
} xfiles_interrupt_t;

#define RESP_CODE_WIDTH 3

// Macros for using XCustom instructions. Four different macros are
// provided depending on whether or not the passed arguments should be
// communicated as registers or immediates.
#define XCUSTOM "custom0"

// Standard macro that passes rd_, rs1_, and rs2_ via registers
#define XFILES_INSTRUCTION(rd_, rs1_, rs2_, funct_)     \
  XFILES_INSTRUCTION_R_R_R(rd_, rs1_, rs2_, funct_)
#define XFILES_INSTRUCTION_R_R_R(rd_, rs1_, rs2_, funct_)               \
  asm volatile (XCUSTOM" %[rd], %[rs1], %[rs2], %[funct]"               \
                : [rd] "=r" (rd_)                                       \
                : [rs1] "r" (rs1_), [rs2] "r" (rs2_), [funct] "i" (funct_))

// Macro to pass rs2_ as an immediate
#define XFILES_INSTRUCTION_R_R_I(rd_, rs1_, rs2_, funct_)               \
  asm volatile (XCUSTOM" %[rd], %[rs1], %[rs2], %[funct]"               \
                : [rd] "=r" (rd_)                                       \
                : [rs1] "r" (rs1_), [rs2] "i" (rs2_), [funct] "i" (funct_))

    // Macro to pass rs1_ and rs2_ as immediates
#define XFILES_INSTRUCTION_R_I_I(rd_, rs1_, rs2_, funct_)               \
  asm volatile (XCUSTOM" %[rd], %[rs1], %[rs2], %[funct]"               \
                : [rd] "=r" (rd_)                                       \
                : [rs1] "i" (rs1_), [rs2] "i" (rs2_), [funct] "i" (funct_))

#endif  // SRC_MAIN_C_XFILES_H_
