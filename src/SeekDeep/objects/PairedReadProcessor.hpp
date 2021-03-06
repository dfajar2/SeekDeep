#pragma once
/*
 * PairedReadProcessor.hpp
 *
 *  Created on: Jan 14, 2018
 *      Author: nick
 */
//
// SeekDeep - A library for analyzing amplicon sequence data
// Copyright (C) 2012-2018 Nicholas Hathaway <nicholas.hathaway@umassmed.edu>,
// Jeffrey Bailey <Jeffrey.Bailey@umassmed.edu>
//
// This file is part of SeekDeep.
//
// SeekDeep is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SeekDeep is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SeekDeep.  If not, see <http://www.gnu.org/licenses/>.
//
#include <bibseq.h>
namespace bibseq {

class PairedReadProcessor{
public:

	enum class AlignOverlapEnd{
		NOOVERHANG,
		R1OVERHANG,
		R2OVERHANG,
		UNHANDLEED
	};

	enum class ReadPairOverLapStatus{
		NOOVERLAP,
		R1BEGINSINR2,
		R1ENDSINR2,
		NONE
	};

	static std::string getOverlapStatusStr(const ReadPairOverLapStatus  status){
		switch (status) {
			case ReadPairOverLapStatus::NOOVERLAP:
				return "NOOVERLAP";
				break;
			case ReadPairOverLapStatus::R1BEGINSINR2:
				return "R1BEGINSINR2";
				break;
			case ReadPairOverLapStatus::R1ENDSINR2:
				return "R1ENDSINR2";
				break;
			default:
				return "NOTHANDLED";//shouldn't be getting here
				break;
		}
	}

	struct ProcessParams{
		uint32_t minOverlap_ = 10;
		double errorAllowed_ = 0.01;
		uint32_t hardMismatchCutOff_ = 10;
		uint32_t checkAmount_ = 100;
		uint32_t testNumber_ = std::numeric_limits<uint32_t>::max();
		bool verbose_ = false;
		bool writeOverHangs_ = false;

		uint32_t r1Trim_ = 0;
		uint32_t r2Trim_ = 0;

	};

	PairedReadProcessor(ProcessParams params);

	void setDefaultConsensusBuilderFunc();
	ProcessParams params_;

	uint64_t guessMaxReadLenFromFile(const SeqIOOptions & inputOpts);

	struct ProcessorOutWriters{
		ProcessorOutWriters();
		ProcessorOutWriters(const OutOptions & outOpts);

		std::unique_ptr<SeqOutput> perfectOverlapCombinedWriter;//(perfectOverlapCombinedOpts);
		std::unique_ptr<SeqOutput> r1EndsInR2CombinedWriter;//(r1EndsInR2CombinedOpts);
		std::unique_ptr<SeqOutput> r1BeginsInR2CombinedWriter;//(r1BeginsInR2CombinedOpts);
		std::unique_ptr<SeqOutput> notCombinedWriter;//(notCombinedOpts);
		std::unique_ptr<SeqOutput> overhangsWriter;//(overhangsOpts);

		void checkWritersSet(const std::string & funcName);
		void unsetWriters();

	};

	struct ProcessedResults {
		uint32_t overlapFail = 0;
		uint32_t overhangFail = 0;
		uint32_t perfectOverlapCombined = 0;
		uint32_t r1EndsInR2Combined = 0;
		uint32_t r1BeginsInR2Combined = 0;
		uint32_t total = 0;

		std::shared_ptr<SeqIOOptions> perfectOverlapCombinedOpts;
		std::shared_ptr<SeqIOOptions> r1EndsInR2CombinedOpts;
		std::shared_ptr<SeqIOOptions> r1BeginsInR2CombinedOpts;
		std::shared_ptr<SeqIOOptions> notCombinedOpts;
		std::shared_ptr<SeqIOOptions> overhangsOpts;

		Json::Value toJson() const;
		Json::Value toJsonCounts() const;
	};

	std::function<void(uint32_t, const seqInfo&,const seqInfo&,std::string&,std::vector<uint32_t>&,aligner&)> addToConsensus;


	ProcessedResults processPairedEnd(
			SeqInput & reader,
			ProcessorOutWriters & writers,
			aligner & alignerObj);

	bool processPairedEnd(
			SeqInput & reader,
			PairedRead & seq,
			ProcessorOutWriters & writers,
			aligner & alignerObj,
			ProcessedResults & res);

};


}  // namespace bibseq




