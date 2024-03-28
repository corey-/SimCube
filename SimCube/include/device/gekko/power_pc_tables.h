#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

struct InstructionInfo
{
    std::string Disassembly;
    std::vector<uint32_t> Operands;

    std::function<void(class IPpcInstructionDecodeHandler&)> Handler;
};

class IPpcInstructionDecodeHandler
{
public:
    virtual ~IPpcInstructionDecodeHandler() = default;

    virtual void HandleTwi() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePscmpu0() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsqlx() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsqstx() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePssum0() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePssum1() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmuls0() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmuls1() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmadds0() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmadds1() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsdiv() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePssub() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsadd() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePssel() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsres() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmul() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsrsqrte() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmsub() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmadd() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsnmsub() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsnmadd() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePscmpo0() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsqlux() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsqstux() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsneg() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePscmpu1() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmr() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePscmpo1() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsnabs() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsabs() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmerge00() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmerge01() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmerge10() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandlePsmerge11() { throw std::runtime_error("Unimplemented opcode"); }
    virtual void HandleDcbzl() { throw std::runtime_error("Unimplemented opcode"); }
};

using PpcOpcode = uint32_t;

[[nodiscard]] std::optional<InstructionInfo> PpcDecode(PpcOpcode opcode);