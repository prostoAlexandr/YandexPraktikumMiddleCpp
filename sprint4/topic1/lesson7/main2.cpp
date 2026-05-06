#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

const std::string HEALTHY_DNA = "ATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCG"
                                "GCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGCTA"
                                "TTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGC";

const std::string CANCER_DNA = "ATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCGATCG"
                               "GCTAGCTAGCTAGCTAGCTATCTAGCTAGCTAGATAGCTAGCTAGCTAGCTAGCTAGCTA"
                               "TTAGCTTAGCTTAGCTTAGCTTTGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGCTTAGC";

constexpr size_t gene_size = 60;

auto gen_wrong_genes(const std::string &healthy_dna, const std::string &cancer_dna) {
    auto h = healthy_dna | std::views::chunk(gene_size) |
             std::views::transform([](auto v) { return std::string(v.begin(), v.end()); }) | std::views::enumerate;
    auto c = cancer_dna | std::views::chunk(gene_size) |
             std::views::transform([](auto v) { return std::string(v.begin(), v.end()); }) | std::views::enumerate;

    std::vector<std::tuple<std::size_t, std::vector<std::tuple<size_t, char>>>> result;
    for (const auto &[ihgene, icgene] : std::views::zip(h, c)) {
        auto hgene = std::get<1>(ihgene) | std::views::enumerate;
        auto cgene = std::get<1>(icgene) | std::views::enumerate;
        std::vector<std::tuple<size_t, char>> diff;
        std::ranges::set_difference(cgene, hgene, std::back_inserter(diff));

        if (!diff.empty()) {
            result.emplace_back(std::get<0>(ihgene), std::move(diff));
        }
    }
    return result;
}

int main() {
    auto result = gen_wrong_genes(HEALTHY_DNA, CANCER_DNA);
    for (const auto &[gene_index, info] : result) {
        std::cout << info.size() << " мутаций в гене #" << gene_index << ":\n";
        for (const auto &[nuc_index, nuc] : info) {
            std::cout << "   Позиция " << nuc_index << ": " << HEALTHY_DNA[gene_index * gene_size + nuc_index] << " → "
                      << nuc << "\n";
        }
    }
}
// ваш код должен приводить к появлению в консоли следующих результатов:
// 2 мутаций в гене #1:
//    Позиция 20: G → T
//    Позиция 33: C → A
// 1 мутаций в гене #2:
//    Позиция 22: A → T