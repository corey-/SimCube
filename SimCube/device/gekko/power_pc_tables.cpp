#include <device/gekko/power_pc_tables.h>
#include <util/endian.h>

#include <array>
#include <functional>
#include <map>
#include <string_view>
#include <variant>

#include "logging/logger.h"

static const LoggerInfo LOGGER{ .SourceName = "PPCDecode" };

enum class DecodeFlags : uint32_t
{
    A,
    B,
    C,
    D,
    S,
    SPR,
    CRFD,
    CRFS,
    IMM,
    SIMM,
    UIMM,
    Displacement_L,
    RC,
    TO,
    W,
    I,
};

static const std::map<DecodeFlags, std::pair<int, uint32_t>> FLAGS_SHIFTS_MASKS = {
    { DecodeFlags::A, { 16, 0b11111 } },  { DecodeFlags::B, { 11, 0b11111 } },
    { DecodeFlags::C, { 6, 0b11111 } },   { DecodeFlags::D, { 21, 0b11111 } },
    { DecodeFlags::S, { 21, 0b11111 } },  { DecodeFlags::SPR, { 11, 0b1111111111 } },
    { DecodeFlags::CRFD, { 23, 0b111 } }, { DecodeFlags::CRFS, { 18, 0b111 } },
    { DecodeFlags::IMM, { 12, 0b1111 } }, { DecodeFlags::SIMM, { 0, 0xFFFF } },
    { DecodeFlags::UIMM, { 0, 0xFFFF } }, { DecodeFlags::Displacement_L, { 0, 0xFFFF } },
    { DecodeFlags::RC, { 0, 0b1 } },      { DecodeFlags::TO, { 21, 0b11111 } },
    { DecodeFlags::W, { 10, 0b1 } },      { DecodeFlags::I, { 7, 0b111 } }
};

struct InstLeaf
{
    std::string_view                                   InstName;
    std::vector<DecodeFlags>                           DecodeFlags;
    std::function<void(IPpcInstructionDecodeHandler&)> Handler;
};

struct InstTablePtr
{
    const struct InstTable*          Next;
    std::function<size_t(PpcOpcode)> NextIndex;
};

using InstTableEntry = std::variant<InstLeaf, InstTablePtr>;

struct InstTable
{
    std::map<size_t, InstTableEntry> Entries;
};

static constexpr auto GetPrimaryOpCode = [](PpcOpcode op) -> size_t { return (op >> 26) & 0b111111; };
static constexpr auto Get4_0Index      = [](PpcOpcode op) -> size_t { return (op >> 6) & 0b11; };
static constexpr auto Get4_6Index      = [](PpcOpcode op) -> size_t { return (op >> 6) & 0b1; };
static constexpr auto Get4_7Index      = [](PpcOpcode op) -> size_t { return (op >> 6) & 0b1; };
static constexpr auto Get4_8Index      = [](PpcOpcode op) -> size_t { return (op >> 6) & 0b1111; };
static constexpr auto Get4_16Index     = [](PpcOpcode op) -> size_t { return (op >> 6) & 0b11111; };
static constexpr auto Get4_21Index     = [](PpcOpcode op) -> size_t { return (op >> 6) & 0b11111; };

#define OPFUN(func) std::mem_fn(&IPpcInstructionDecodeHandler::func)

static const InstTable PPC_OP_4_0_TABLE = { {
    //
    { 0, InstLeaf{ "ps_cmpu0", { DecodeFlags::CRFD, DecodeFlags::A, DecodeFlags::B }, OPFUN(HandlePscmpu0) } },
    { 1, InstLeaf{ "ps_cmpo0", { DecodeFlags::CRFD, DecodeFlags::A, DecodeFlags::B }, OPFUN(HandlePscmpo0) } },
    { 2, InstLeaf{ "ps_cmpu1", { DecodeFlags::CRFD, DecodeFlags::A, DecodeFlags::B }, OPFUN(HandlePscmpu1) } },
    { 3, InstLeaf{ "ps_cmpo1", { DecodeFlags::CRFD, DecodeFlags::A, DecodeFlags::B }, OPFUN(HandlePscmpo1) } },
    //
} };

static const InstTable PPC_OP_4_6_TABLE = { {
    //
    { 0,
      InstLeaf{ "psq_lx",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::W, DecodeFlags::I },
                OPFUN(HandlePsqlx) } },
    { 1,
      InstLeaf{ "psq_lux",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::W, DecodeFlags::I },
                OPFUN(HandlePsqlux) } },
    //
} };

static const InstTable PPC_OP_4_7_TABLE = { {
    //
    { 0,
      InstLeaf{ "psq_stx",
                { DecodeFlags::S, DecodeFlags::A, DecodeFlags::B, DecodeFlags::W, DecodeFlags::I },
                OPFUN(HandlePsqstx) } },
    { 1,
      InstLeaf{ "psq_stux",
                { DecodeFlags::S, DecodeFlags::A, DecodeFlags::B, DecodeFlags::W, DecodeFlags::I },
                OPFUN(HandlePsqstux) } },
    //
} };

static const InstTable PPC_OP_4_8_TABLE = { {
    //
    { 1, InstLeaf{ "ps_neg", { DecodeFlags::D, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsneg) } },
    { 2, InstLeaf{ "ps_mr", { DecodeFlags::D, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsmr) } },
    { 4, InstLeaf{ "ps_nabs", { DecodeFlags::D, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsnabs) } },
    { 8, InstLeaf{ "ps_abs", { DecodeFlags::D, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsabs) } },
    //
} };

static const InstTable PPC_OP_4_16_TABLE = { {
    //
    { 16,
      InstLeaf{ "ps_merge00",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC },
                OPFUN(HandlePsmerge00) } },
    { 17,
      InstLeaf{ "ps_merge01",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC },
                OPFUN(HandlePsmerge01) } },
    { 19,
      InstLeaf{ "ps_merge11",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC },
                OPFUN(HandlePsmerge11) } },
    //
} };

static const InstTable PPC_OP_4_21_TABLE = { {
    //
    { 0,
      InstLeaf{ "ps_add", { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsadd) } },
    { 18,
      InstLeaf{ "ps_merge10",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC },
                OPFUN(HandlePsmerge10) } },
    //
} };

static const InstTable PPC_OP_4_TABLE = { {
    //
    { 0, InstTablePtr{ &PPC_OP_4_0_TABLE, Get4_0Index } },
    { 6, InstTablePtr{ &PPC_OP_4_6_TABLE, Get4_6Index } },
    { 7, InstTablePtr{ &PPC_OP_4_7_TABLE, Get4_7Index } },
    { 8, InstTablePtr{ &PPC_OP_4_8_TABLE, Get4_8Index } },
    { 10,
      InstLeaf{ "ps_sum0",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePssum0) } },
    { 11,
      InstLeaf{ "ps_sum1",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePssum1) } },
    { 12,
      InstLeaf{ "ps_muls0",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsmuls0) } },
    { 13,
      InstLeaf{ "ps_muls1",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsmuls1) } },
    { 14,
      InstLeaf{ "ps_madds0",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsmadds0) } },
    { 15,
      InstLeaf{ "ps_madds1",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsmadds1) } },
    { 16, InstTablePtr{ &PPC_OP_4_16_TABLE, Get4_16Index } },
    { 18,
      InstLeaf{ "ps_div", { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsdiv) } },
    { 20,
      InstLeaf{ "ps_sub", { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePssub) } },
    { 21, InstTablePtr{ &PPC_OP_4_21_TABLE, Get4_21Index } },
    { 22, InstLeaf{ "dcbz_l", { DecodeFlags::A, DecodeFlags::B }, OPFUN(HandleDcbzl) } },
    { 23,
      InstLeaf{ "ps_sel",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePssel) } },
    { 24, InstLeaf{ "ps_res", { DecodeFlags::D, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsres) } },
    { 25,
      InstLeaf{ "ps_mul", { DecodeFlags::D, DecodeFlags::A, DecodeFlags::C, DecodeFlags::RC }, OPFUN(HandlePsmul) } },
    { 26, InstLeaf{ "ps_rsqrte", { DecodeFlags::D, DecodeFlags::B, DecodeFlags::RC }, OPFUN(HandlePsrsqrte) } },
    { 28,
      InstLeaf{ "ps_msub",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsmsub) } },
    { 29,
      InstLeaf{ "ps_madd",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsmadd) } },
    { 30,
      InstLeaf{ "ps_nmsub",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsnmsub) } },
    { 31,
      InstLeaf{ "ps_nmadd",
                { DecodeFlags::D, DecodeFlags::A, DecodeFlags::B, DecodeFlags::C, DecodeFlags::RC },
                OPFUN(HandlePsnmadd) } },
    //
} };

static const InstTable PPC_BASE_TABLE = { {
    //
    { 3, InstLeaf{ "twi", { DecodeFlags::TO, DecodeFlags::A, DecodeFlags::SIMM }, OPFUN(HandleTwi) } },
    { 4, InstTablePtr{ &PPC_OP_4_TABLE, GetPrimaryOpCode } }
    //
} };

InstructionInfo FillOutInstInfo(const InstLeaf& inst, PpcOpcode data)
{
    InstructionInfo info{ .Disassembly = "TODO", .Operands = {}, .Handler = inst.Handler };

    for (const auto flag : inst.DecodeFlags)
    {
        const auto& [shift, mask] = FLAGS_SHIFTS_MASKS.at(flag);

        info.Operands.push_back((data >> shift) & mask);
    }
    return info;
}

std::optional<InstructionInfo> PpcDecode(PpcOpcode opcode)
{
    auto             index = GetPrimaryOpCode(opcode);
    const InstTable* table = &PPC_BASE_TABLE;

    while (table->Entries.contains(index))
    {
        const auto& entry = table->Entries.at(index);

        if (const InstLeaf* leaf = get_if<InstLeaf>(&entry))
        {
            return FillOutInstInfo(*leaf, opcode);
        }

        const auto& [nextTable, nextIndex] = get<InstTablePtr>(entry);

        index = nextIndex(opcode);
        table = nextTable;
    }

#ifndef NDEBUG
    Log(LOGGER, LogLevel::ERROR, "Invalid Opcode: {:x}", opcode);
    std::exit(-1);
#endif

    return std::nullopt;
}
