#ifndef LINEAR_PROBING_HPP_
#define LINEAR_PROBING_HPP_

#include <array>
#include <cstdlib>
#include <limits>
#include <optional>
#include <type_traits>

namespace komori {

template <class Key, class Value, std::size_t N>
class LinearProbingTable {
  static_assert(N > 0 && (N & (N - 1)) == 0, "N must be a power of 2");
  static_assert(std::is_integral_v<Key> && std::is_unsigned_v<Key>);
  static_assert(std::is_trivial_v<Value>);

  using Index = Key;
  using Id = Key;

 public:
  static constexpr Key kInvalidKey = std::numeric_limits<Key>::max();

  constexpr LinearProbingTable() {
    std::fill(values_.begin(), values_.end(),
              std::make_tuple(kInvalidKey, Value{}, Id{}));
  }

  constexpr void Insert(Key key, Value value) noexcept {
    Index index = static_cast<Index>(key & kKeyMask);
    while (std::get<0>(values_[index]) != kInvalidKey) {
      index = Next(index);
    }

    values_[index] = std::make_tuple(key, value, id_end_);
    id_end_ = Next(id_end_);
  }

  constexpr std::optional<Key> LookUp(Key key) noexcept {
    Index index = static_cast<Index>(key & kKeyMask);
    while (std::get<0>(values_[index]) != kInvalidKey) {
      if (std::get<0>(values_[index]) == key) {
        return {std::get<1>(values_[index])};
      }

      index = Next(index);
    }

    return std::nullopt;
  }

  constexpr void CollectGarbage(std::size_t remove_num) noexcept {
    const auto new_id_begin = (id_begin_ + remove_num) & kKeyMask;
    for (Index i = 0; i <= kKeyMask; ++i) {
      const auto key = std::get<0>(values_[i]);
      const auto id = std::get<2>(values_[i]);

      if (key == kInvalidKey) {
        continue;
      }

      if ((id_begin_ <= id && id < new_id_begin) ||
          (id < new_id_begin && new_id_begin <= id_begin_) ||
          (new_id_begin <= id_begin_ && id_begin_ <= id)) {
        std::get<0>(values_[i]) = kInvalidKey;
      }
    }
    id_begin_ = new_id_begin;

    for (Index index = 0; index <= kKeyMask; ++index) {
      const auto key = std::get<0>(values_[index]);
      if (key != kInvalidKey) {
        auto new_index = static_cast<Index>(key & kKeyMask);
        while (new_index != index) {
          if (std::get<0>(values_[new_index]) == kInvalidKey) {
            std::swap(values_[index], values_[new_index]);
            break;
          }

          new_index = Next(new_index);
        }
      }
    }
  }

 private:
  static constexpr Index kKeyMask = static_cast<Index>(N - 1);

  constexpr Index Next(Index index) const noexcept {
    return (index + 1) & kKeyMask;
  }

  std::array<std::tuple<Key, Value, Id>, N> values_;

  Id id_begin_{};
  Id id_end_{};
};

}  // namespace komori

#endif  // LINEAR_PROBING_HPP_