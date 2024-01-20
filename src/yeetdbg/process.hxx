#include "elf/elf++.hh"
#include <bits/types/siginfo_t.h>
#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include <dwarf/dwarf++.hh>

#ifndef PROCESS_HXX
#define PROCESS_HXX

#define REGISTER_NOT_FOUND_EX 20

namespace yeetdbg {
  enum STATUS {
    RUNNING,
    PAUSED,
    DEAD,
  };

  enum class reg {
    r15, r14, r13, r12,
    rbp, rbx, r11, r10,
    r9, r8, rax, rcx,
    rdx, rsi, rdi, orig_rax,
    rip, cs, eflags, rsp,
    ss, fs_base, gs_base, ds,
    es, fs, gs,
  };

  constexpr size_t reg_count = 27;

  struct reg_descriptor {
    reg r;
    int dwarfno;
    std::string name;
  };

  const std::array<reg_descriptor, reg_count> g_registers {{
      {reg::r15,      15, "r15"},
      {reg::r14,      14, "r14"},
      {reg::r13,      13, "r13"},
      {reg::r12,      12, "r12"},
      {reg::rbp,       6, "rbp"},
      {reg::rbx,       3, "rbx"},
      {reg::r11,      11, "r11"},
      {reg::r10,      10, "r10"},
      {reg::r9,        9, "r9"},
      {reg::r8,        8, "r8"},
      {reg::rax,       0, "rax"},
      {reg::rcx,       2, "rcx"},
      {reg::rdx,       1, "rdx"},
      {reg::rsi,       4, "rsi"},
      {reg::rdi,       5, "rdi"},
      {reg::orig_rax, -1, "orig_rax"},
      {reg::rip,      -1, "rip"},
      {reg::cs,       51, "cs"},
      {reg::eflags,   49, "eflags"},
      {reg::rsp,       7, "rsp"},
      {reg::ss,       52, "ss"},
      {reg::fs_base,  58, "fs_base"},
      {reg::gs_base,  59, "gs_base"},
      {reg::ds,       53, "ds"},
      {reg::es,       50, "es"},
      {reg::fs,       54, "fs"},
      {reg::gs,       55, "gs"},
  }};

  class Process {
    public:
      Process() {};
      Process(std::string file, std::vector<std::string> params): m_file(file), m_params(params) { initialize(); }
      void start();
      void resume();
      int wait(int options);

      uint64_t read_quad(uint64_t addr);
      void write_quad(uint64_t addr, uint64_t data);

      uint64_t get_reg_value(reg r);
      void set_reg_value(reg r, uint64_t data);
      uint64_t get_reg_value(int r);
      void set_reg_value(int r, uint64_t data);
      uint64_t get_reg_value(std::string r);
      void set_reg_value(std::string r, uint64_t data);

      void step_instruction(uint8_t count = 1);

      uint64_t resolve_input(std::string input);
      dwarf::die get_current_function();

      uint64_t offset_by_base(uint64_t a) { return m_base + a; }
      uint64_t negate_base(uint64_t a) { return a - m_base; }

      void handle_signal(siginfo_t signal);

      std::vector<std::string> get_src_for_address(uint64_t addr, int surround_line_count = 2);

      siginfo_t get_siginfo();

    private:
      uint64_t get_reg_by_idx(int r);
      void set_reg_by_idx(int r, uint64_t data);

      void read_mappings();
      void initialize();

      dwarf::die get_die_at_addr(uint64_t);
      dwarf::line_table::iterator get_line_table_entry(uint64_t addr);

      uint64_t m_base = 0;
      int m_pid;
      std::string m_file;
      std::vector<std::string> m_params;
      STATUS m_status;
      std::string maps;
      elf::elf m_elf;
      dwarf::dwarf m_dwarf;
  };
}

#endif
