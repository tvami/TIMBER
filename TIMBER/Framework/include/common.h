#ifndef _TIMBER_COMMON
#define _TIMBER_COMMON

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <boost/filesystem.hpp>

#include <cmath>
#include <cstdlib>
#include <ROOT/RVec.hxx>
#include <TMath.h>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PtEtaPhiM4D.h>
#include <Math/Vector4Dfwd.h>

using namespace ROOT::VecOps;
/**
 * @namespace hardware
 * @brief Namespace for common physics functions.
 */
namespace hardware {  
    /**
     * @brief Calculate the difference in \f$\phi\f$.
     * 
     * @param phi1 
     * @param phi2 
     * @return float Difference in \f$\phi\f$.
     */
    float DeltaPhi(float phi1,float phi2);
    /**
     * @brief Calculate \f$\Delta R\f$ between two vectors.
     * 
     * @param v1 
     * @param v2 
     * @return float 
     */
    float DeltaR(ROOT::Math::PtEtaPhiMVector v1, ROOT::Math::PtEtaPhiMVector v2);
    /**
     * @brief Calculate \f$\Delta R\f$ between two objects.
     * 
     * @param in1 
     * @param in2 
     * @return float 
     */
    template<class T1, class T2>
    float DeltaR(T1 in1, T2 in2) {
        ROOT::Math::PtEtaPhiMVector v1(in1.pt, in1.eta, in1.phi, in1.mass);
        ROOT::Math::PtEtaPhiMVector v2(in2.pt, in2.eta, in2.phi, in2.mass);
        float deta = v1.Eta()-v2.Eta();
        float dphi = DeltaPhi(v1.Phi(),v2.Phi());
        return sqrt(deta*deta+dphi*dphi);
    }
    /**
     * @brief Create a ROOT::Math::PtEtaPhiMVector.
     * 
     * @param pt 
     * @param eta 
     * @param phi 
     * @param m 
     * @return ROOT::Math::PtEtaPhiMVector 
     */
    ROOT::Math::PtEtaPhiMVector TLvector(float pt,float eta,float phi,float m);
    /**
     * @brief Create a vector of ROOT::Math::PtEtaPhiMVectors.
     * 
     * @param pt 
     * @param eta 
     * @param phi 
     * @param m 
     * @return RVec<ROOT::Math::PtEtaPhiMVector> 
     */
    RVec<ROOT::Math::PtEtaPhiMVector> TLvector(RVec<float> pt,RVec<float> eta,RVec<float> phi,RVec<float> m);
    /**
     * @brief Create a ROOT::Math::PtEtaPhiMVectors.
     * 
     * @param obj
     * @return ROOT::Math::PtEtaPhiMVector
     */
    template<class T>
    ROOT::Math::PtEtaPhiMVector TLvector(T obj) {
        ROOT::Math::PtEtaPhiMVector v (obj.pt, obj.eta, obj.phi, obj.mass);
        return v;
    }
    /**
     * @brief Create a vector of ROOT::Math::PtEtaPhiMVectors.
     * 
     * @param objs 
     * @return RVec<ROOT::Math::PtEtaPhiMVector> 
     */
    template<class T>
    RVec<ROOT::Math::PtEtaPhiMVector> TLvector(RVec<T> objs) {
        RVec<ROOT::Math::PtEtaPhiMVector> vs;
        vs.reserve(objs.size());
        for (size_t i = 0; i < objs.size(); i++) {
            vs.emplace_back(objs[i].pt, objs[i].eta, objs[i].phi, objs[i].mass);
        }
        return vs;
    }

    /**
     * @brief Calculate the transverse mass from MET \f$p_T\f$ and \f$\eta\f$
     * and an object's \f$p_T\f$ and \f$\eta\f$.
     * 
     * @param MET_pt 
     * @param obj_pt 
     * @param MET_phi 
     * @param obj_phi 
     * @return float 
     */
    float transverseMass(float MET_pt, float obj_pt, float MET_phi, float obj_phi);

    /**
     * @brief Calculates the invariant mass of a vector of Lorentz vectors
     * (ROOT::Math::PtEtaPhiMVector). Note that this is an alternative
     * to [ROOT::VecOps::InvariantMasses()](https://root.cern/doc/master/namespaceROOT_1_1VecOps.html#a2c531eae910edad48bbf7319cc6d7e58)
     * which does not need the intermediate Lorentz vector.
     * 
     * @param vects 
     * @return double 
     */
    double invariantMass(RVec<ROOT::Math::PtEtaPhiMVector> vects);
}

namespace Pythonic {
    /**
     * @brief Python-like range function.
     * https://stackoverflow.com/questions/13152252/is-there-a-compact-equivalent-to-python-range-in-c-stl
     * 
     * @tparam IntType 
     * @param start Starting point, inclusive.
     * @param stop Stopping point, exclusive.
     * @param step Defaults to 1.
     * @return std::vector<IntType> Vector of range with provided step.
     */
    template <typename IntType>
    std::vector<IntType> Range(IntType start, IntType stop, IntType step) {
        if (step == IntType(0)) {
            throw std::invalid_argument("step for range must be non-zero");
        }

        std::vector<IntType> result;
        IntType i = start;
        while ((step > 0) ? (i < stop) : (i > stop)) {
            result.push_back(i);
            i += step;
        }

        return result;
    }

    /**
     * @brief Python-like range function with step 1.
     * https://stackoverflow.com/questions/13152252/is-there-a-compact-equivalent-to-python-range-in-c-stl
     * 
     * @tparam IntType 
     * @param start Starting point, inclusive.
     * @param stop Stopping point, exclusive.
     * @return std::vector<IntType> Vector of range with step of 1.
     */
    template <typename IntType>
    std::vector<IntType> Range(IntType start, IntType stop) {
        return Range(start, stop, IntType(1));
    }

    /**
     * @brief Python-like range function with step 1 and start assumed to be 0.
     * https://stackoverflow.com/questions/13152252/is-there-a-compact-equivalent-to-python-range-in-c-stl
     * 
     * @tparam IntType 
     * @param start Starting point, inclusive.
     * @return std::vector<IntType> Vector of range with step of 1 and start of 0.
     */
    template <typename IntType>
    std::vector<IntType> Range(IntType stop) {
        return Range(IntType(0), stop, IntType(1));
    }

    /**
     * @brief Python-like string splitter based on a delimiter.
     * 
     * Adapted from http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
     * 
     * @param str String to split.
     * @param delim Char to split around.
     * @return std::vector<std::string> Vector of pieces split around delimiter.
     */
    std::vector<std::string> Split(const std::string& str, char delim = ' ');

    // Personal
    /**
     * @brief Checks for object in a list.
     * 
     * @tparam T 
     * @param obj Object to look for.
     * @param list List to look in.
     * @return bool True or false based on whether object is found in list.
     */
    template<typename T>
    bool InList(T obj, std::vector<T> list) {
        bool out;
        auto pos = std::find(std::begin(list), std::end(list), obj);
        if (pos != std::end(list)){
            out = true;
        } else {out = false;}
        return out;
    }

    /**
     * @brief Check for string in another string.
     * 
     * @param sub Substring to look for.
     * @param main String to look in.
     * @return out  True or false based on whether sub is found in main.
     * @return false 
     */
    bool InString(std::string sub, std::string main);

    /**
     * @brief Extend vector with another vector (modifies base in-place).
     * 
     * @tparam T 
     * @param base Modif
     * @param extension 
     */
    template<typename T>
    void Extend(std::vector<T> base, std::vector<T> extension) {
        for (int i = 0; i < extension.size(); i++) {
            base.push_back(extension.at(i));
        }
    }

    /**
     * @brief Checks if directory exists.
     * From https://stackoverflow.com/questions/3828192/checking-if-a-directory-exists-in-unix-system-call
     * 
     * @param dirname 
     * @return exists 
     */
    bool IsDir(char* dirname);
    /**
     * @brief Prints command and executes via std::system.
     * 
     * @param cmd 
     */
    void Execute(std::string cmd);
}

/**
 * @brief Streams a tgz file (tarname) and searches for internalFile
 * within the tgz. Returns a string of the internalFile contents.
 * 
 * @param tarname 
 * @param internalFile 
 * @return std::string 
 */
std::string ReadTarFile(std::string tarname, std::string internalFile);

/**
 * @brief Creates a temporary directory that is destroyed on delete.
 */
class TempDir {
    private:
        boost::filesystem::path _path;
        std::vector<std::string> _filesSaved;

    public:
        /**
         * @brief Construct a new Temp Dir object
         * 
         */
        TempDir();
        /**
         * @brief Destroy the Temp Dir object
         * 
         */
        ~TempDir();
        /**
         * @brief Write a string (in) to a file (filename) within the 
         * temporary directory.
         * 
         * @param filename 
         * @param in 
         * @return std::string 
         */
        std::string Write(std::string filename, std::string in);
};
#endif