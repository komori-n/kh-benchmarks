#ifndef KOMORI_VISIT_HISTORY_HPP_
#define KOMORI_VISIT_HISTORY_HPP_

#include <optional>
#include <unordered_map>

namespace komori {
namespace old {
using Key = std::uint64_t;
using Hand = std::uint32_t;
using Depth = std::int32_t;

class VisitHistory {
 public:
  void Visit(Key board_key, Hand hand, Depth depth) {
    visited_.emplace(board_key, std::make_pair(hand, depth));
  }

  void Leave(Key board_key, Hand hand) {
    auto [begin, end] = visited_.equal_range(board_key);
    for (auto itr = begin; itr != end; ++itr) {
      if (itr->second.first == hand) {
        visited_.erase(itr);
        return;
      }
    }
  }

  std::optional<Depth> Contains(Key board_key, Hand hand) const {
    const auto [begin_itr, end_itr] = visited_.equal_range(board_key);
    for (auto itr = begin_itr; itr != end_itr; ++itr) {
      const auto& [bk, history_hd] = *itr;
      const auto& [history_hand, history_depth] = history_hd;
      if (history_hand == hand) {
        return history_depth;
      }
    }

    return std::nullopt;
  }

 private:
  using HandDepthPair = std::pair<Hand, Depth>;
  std::unordered_multimap<Key, HandDepthPair> visited_;
};
}  // namespace old
}  // namespace komori

#endif  // KOMORI_VISIT_HISTORY_HPP_
