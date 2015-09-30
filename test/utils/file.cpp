/* file.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include "file.hpp"

Eigen::MatrixXd get_matrix_from_csv(std::string const & csv_filename) {
    io::CSVReader<3> csv(csv_filename);
    csv.read_header(io::ignore_extra_column, "out_x1_hex", "out_x2_hex", "out_x3_hex");
    std::string x; std::string y; std::string z;
    Eigen::MatrixXd matrix(get_line_count(csv_filename)-1, 3);
    int line_count = 0;
    while (csv.read_row(x, y, z)) {
        unsigned int buffer;
        std::stringstream ss;
        ss << std::hex << x;
        ss >> buffer;
        matrix (line_count, 0) = reinterpret_cast<double&>(buffer);
        ss.str("");
        ss << std::hex << y;
        ss >> buffer;
        matrix (line_count, 1) = reinterpret_cast<double&>(buffer);
        ss.str("");
        ss << std::hex << z;
        ss >> buffer;
        matrix (line_count, 2) = reinterpret_cast<double&>(buffer);
        ++line_count;
    }
    return matrix;
}

Frame get_frame_from_csv(const std::string & csv_filename) {
    io::CSVReader<3> csv(csv_filename);
    csv.read_header(io::ignore_extra_column, "out_x1_hex", "out_x2_hex", "out_x3_hex");
    std::string x; std::string y; std::string z;
    std::vector<double> x_vector;
    std::vector<double> y_vector;
    std::vector<double> z_vector;
    int line_count = 0;
    while (csv.read_row(x, y, z)) {
        unsigned int buffer;
        std::stringstream ss_x;
        ss_x << std::hex << x;
        ss_x >> buffer;
        float tmp_x = reinterpret_cast<float&>(buffer);
        std::stringstream ss_y;
        ss_y << std::hex << y;
        ss_y >> buffer;
        float tmp_y = reinterpret_cast<float&>(buffer);
        std::stringstream ss_z;
        ss_z << std::hex << z;
        ss_z >> buffer;
        float tmp_z = reinterpret_cast<float&>(buffer);
        x_vector.push_back((double)(tmp_x));
        y_vector.push_back((double)(tmp_y));
        z_vector.push_back((double)(tmp_z));
        ++line_count;
    }
    return Frame(x_vector, y_vector, z_vector);
}

Frame get_frame_from_csv_not_hex(const std::string & csv_filename) {
    io::CSVReader<3> csv(csv_filename);
    csv.read_header(io::ignore_extra_column, "x1", "x2", "x3");
    std::string x; std::string y; std::string z;
    std::vector<double> x_vector;
    std::vector<double> y_vector;
    std::vector<double> z_vector;
    while (csv.read_row(x, y, z)) {
        x_vector.push_back(std::stod(x));
        y_vector.push_back(std::stod(y));
        z_vector.push_back(std::stod(z));
    }
    return Frame(x_vector, y_vector, z_vector);
}

std::vector<Range> get_ranges_from_csv(const std::string & csv_path){
    io::CSVReader<2> range_file(csv_path);
    int start;
    int end;
    range_file.read_header(io::ignore_extra_column, "start", "end");
    std::vector<Range> ranges;
    while(range_file.read_row(start, end)){
        Range range = {start, end, end - start};
        ranges.push_back(range);
    }
    return ranges;
}

Eigen::VectorXd get_vector_from_csv(const std::string & csv_path, const std::string & column_name)
{
    std::vector<std::string> vec_tmp = get_std_vector_from_csv<std::string>(csv_path, column_name);
    size_t vec_len = vec_tmp.size();
    Eigen::VectorXd result(vec_len);
    for (size_t i=0; i<vec_len; ++i) {
        result(i) = cast_hex_to_double(vec_tmp.at(i));
    }
    return result;
}

Eigen::MatrixXd get_sym_matrix_from_file(const std::string & matrix_file_path, int dim, char delim, bool is_hex)
{
    Eigen::MatrixXd return_matrix(dim,dim);

    std::ifstream matrix_file;
    matrix_file.open(matrix_file_path);
    if (matrix_file.is_open() == false)
    {
        std::cerr << matrix_file_path << " not found!" << std::endl;
        return return_matrix;
    }

    std::string row;
    size_t cols=0;
    size_t rows=0;

    while (std::getline(matrix_file,row)){

        cols=0;
        size_t begin=row.find_first_not_of(' '); // ignore spaces at start
        size_t delimiter_pos;
        std::string entry;
        //std::cout << "begin: " << begin << std::endl;
        do {
            delimiter_pos = row.find(delim, begin);
            entry = row.substr(begin, delimiter_pos-begin);
            if (is_hex)
                return_matrix(rows,cols) = cast_hex_to_double(entry);
            else
            {
                //std::cout << entry << " ";
                return_matrix(rows,cols) = std::atof(entry.c_str());
            }

            begin=delimiter_pos+1;
            cols++;
        } while (delimiter_pos != std::string::npos);
        rows++;
    }
    return return_matrix;
}

NormalModeMeanSquareFluctuationCalculator get_nmmsfc_for_testing(const std::string& pdb_path,
                                                                const std::string& dssp_path,
                                                                const std::string& test_prefix)
{
    const std::string test_dir = "test/resources/calnmodemsfss/";


    io::CSVReader<28> reach_data(test_dir + test_prefix + "_calnmodemsfss.csv");
    reach_data.read_header(io::ignore_extra_column,"nres","nvar","ave","mass","npa","npb",
                           "alpha_info","beta_info","numbetapair",
                           "k12","k13","k14","anb","bnb","anb2","bnb2","k12cis",
                           "ka12","ka13","ka14","ka15","kb12","kb13","kb14","kbpair",
                           "hessian","xxcom","eigvec");

    int nres,nvar,npa,npb,numbetapair;
    std::string k12,k13,k14,anb,bnb,anb2,bnb2,k12cis,ka12,ka13,ka14,ka15,
            kb12,kb13,kb14,kbpair;
    std::string ave_ref,mass_ref, alpha_ref,beta_ref,hessian_ref,xxcom_ref,eigvec_ref;

    reach_data.read_row(nres,nvar,ave_ref,mass_ref,npa,npb,
                        alpha_ref,beta_ref,numbetapair,
                        k12,k13,k14,anb,bnb,anb2,bnb2,k12cis,
                        ka12,ka13,ka14,ka15,kb12,kb13,kb14,kbpair,
                        hessian_ref,xxcom_ref,eigvec_ref);

    Eigen::VectorXd ave = get_vector_from_csv(test_dir + ave_ref,"ave");

    std::shared_ptr<PDB> pdb(new PDB(pdb_path));
    std::shared_ptr<DSSP> dssp(new DSSP(dssp_path));
    Protein protein(pdb,dssp);

    std::shared_ptr<ProteinSegment> complete_protein_segment(std::make_shared<ProteinSegment>(protein.get_residues(),1,151,StructureType::COMPLETE_PROTEIN));
    complete_protein_segment->add_displacement_vector(ave);

    ForceConstantSelector kk_selector(cast_hex_to_double(k12), cast_hex_to_double(k13),
                           cast_hex_to_double(k14), cast_hex_to_double(anb),
                           cast_hex_to_double(bnb), cast_hex_to_double(anb2),
                           cast_hex_to_double(bnb2), cast_hex_to_double(k12cis),
                           cast_hex_to_double(ka12), cast_hex_to_double(ka13),
                           cast_hex_to_double(ka14), cast_hex_to_double(ka15),
                           cast_hex_to_double(kb12), cast_hex_to_double(kb13),
                           cast_hex_to_double(kb14), cast_hex_to_double(kbpair),true);

    NormalModeMeanSquareFluctuationCalculator nmmsfc(protein.get_residues());

    nmmsfc.calculate(protein, 120.0, kk_selector, complete_protein_segment);

    return nmmsfc;
}

std::vector<std::string> glob(const std::string& pat)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> ret;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
