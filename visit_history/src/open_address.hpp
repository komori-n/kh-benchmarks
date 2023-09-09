#ifndef KOMORI_OPEN_ADDRESS_HISTORY_HPP_
#define KOMORI_OPEN_ADDRESS_HISTORY_HPP_

#include <array>
#include <cstdint>
#include <optional>

namespace komori {
namespace open_addressing {
using Key = std::uint64_t;
using Hand = std::uint32_t;
using Depth = std::int32_t;

constexpr inline Hand kNullHand = std::numeric_limits<Hand>::max();

template <std::uint32_t kArraySize>
class VisitHistory {
  static_assert(kArraySize > 0, "kArraySize must be positive");
  static_assert((kArraySize & (kArraySize - 1)) == 0,
                "kArraySize must be power of 2");

 public:
  void Visit(Key board_key, Hand hand, Depth depth) {
    auto index = DefaultIndex(board_key);
    while (entries_[index].hand != kNullHand) {
      index = (index + 1) % kArraySize;
    }

    entries_[index] = Entry{hand, depth, board_key};
    index_stack_[stack_len_++] = index;
  }

  void Leave(Key /* board_key */, Hand /* hand */) {
    const auto index = index_stack_[--stack_len_];
    entries_[index].hand = kNullHand;
  }

  std::optional<Depth> Contains(Key board_key, Hand hand) const {
    auto index = DefaultIndex(board_key);
    while (entries_[index].hand != kNullHand) {
      const auto& entry = entries_[index];
      if (entry.board_key == board_key && entry.hand == hand) {
        return {entry.depth};
      }

      index = (index + 1) % kArraySize;
    }
    return std::nullopt;
  }

 private:
  static constexpr Key kKeyIndexMask = static_cast<Key>(kArraySize - 1);

  std::uint32_t DefaultIndex(Key board_key) const {
    return board_key & kKeyIndexMask;
  }

  struct Entry {
    Hand hand{kNullHand};
    Depth depth{};
    Key board_key{};
  };

  std::array<Entry, kArraySize> entries_{};
  std::array<std::uint32_t, kArraySize> index_stack_{};
  std::uint32_t stack_len_{};
};
}  // namespace open_addressing
}  // namespace komori

#endif  // KOMORI_OPEN_ADDRESS_HISTORY_HPP_