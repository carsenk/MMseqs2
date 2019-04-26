// Ported from blast2lca
// https://github.com/emepyc/Blast2lca
// Originally licensed under GPLv2 or later

#ifndef MMSEQS_NCBITAXONOMY_H
#define MMSEQS_NCBITAXONOMY_H

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

using TaxID = int;

struct TaxonNode {
    int id;
    TaxID taxId;
    TaxID parentTaxId;
    std::string rank;
    std::string name;
    std::vector<TaxID> children;

    TaxonNode(int id, TaxID taxId, TaxID parentTaxId, const std::string& rank)
            : id(id), taxId(taxId), parentTaxId(parentTaxId), rank(rank), name("") {};
};


class NcbiTaxonomy {
public:
    NcbiTaxonomy(const std::string &namesFile,  const std::string &nodesFile,
                 const std::string &mergedFile);
    ~NcbiTaxonomy();

    TaxonNode const * LCA(const std::vector<TaxID>& taxa) const;
    TaxID LCA(TaxID taxonA, TaxID taxonB) const;
    std::vector<std::string> AtRanks(TaxonNode const * node, const std::vector<std::string> &levels) const;
    std::map<std::string, std::string> AllRanks(TaxonNode const *node) const;
    std::string taxLineage(TaxonNode const *node);

    bool IsAncestor(TaxID ancestor, TaxID child);
    TaxonNode const* taxonNode(TaxID taxonId, bool fail = true) const;
    std::unordered_map<TaxID, unsigned int> getCladeCounts(std::unordered_map<TaxID, unsigned int>& taxonCounts, TaxID taxon = 1) const;

private:
    void InitLevels();
    size_t loadNodes(const std::string &nodesFile);
    size_t loadMerged(const std::string &mergedFile);
    void loadNames(const std::string &namesFile);
    void elh(int node, int level);
    void InitRangeMinimumQuery();
    int nodeId(TaxID taxId) const;
    bool nodeExists(TaxID taxId) const;

    int RangeMinimumQuery(int i, int j) const;
    int lcaHelper(int i, int j) const;

    unsigned int cladeSummation(const std::unordered_map<TaxID, unsigned int>& taxonCounts,
                                std::unordered_map<TaxID, unsigned int>& cladeCounts, TaxID taxID) const;

    std::vector<TaxonNode> taxonNodes;
    std::vector<int> D; // maps from taxID to node ID in taxonNodes
    std::vector<int> E; // for Euler tour sequence (size 2N-1)
    std::vector<int> L; // Level of nodes in tour sequence (size 2N-1)
    int* H;
    int **M;
    size_t maxNodes;

    std::map<std::string, int> sortedLevels;
};

#endif
