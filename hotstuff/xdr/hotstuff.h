// -*- C++ -*-
// Automatically generated from hotstuff/xdr/hotstuff.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_HOTSTUFF_XDR_HOTSTUFF_H_INCLUDED__
#define __XDR_HOTSTUFF_XDR_HOTSTUFF_H_INCLUDED__ 1

#include <xdrpp/types.h>

#include "hotstuff/xdr/types.h"

namespace hotstuff {

using ReplicaIDBitMap = uint32;

struct PartialCertificateWire {
  Hash hash{};
  Signature sig{};

  PartialCertificateWire() = default;
  template<typename _hash_T,
           typename _sig_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _hash_T>::value
                          && std::is_constructible<Signature, _sig_T>::value
                         >::type>
  explicit PartialCertificateWire(_hash_T &&_hash,
                                  _sig_T &&_sig)
    : hash(std::forward<_hash_T>(_hash)),
      sig(std::forward<_sig_T>(_sig)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::PartialCertificateWire>
  : xdr_struct_base<field_ptr<::hotstuff::PartialCertificateWire,
                              decltype(::hotstuff::PartialCertificateWire::hash),
                              &::hotstuff::PartialCertificateWire::hash>,
                    field_ptr<::hotstuff::PartialCertificateWire,
                              decltype(::hotstuff::PartialCertificateWire::sig),
                              &::hotstuff::PartialCertificateWire::sig>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::PartialCertificateWire &obj) {
    archive(ar, obj.hash, "hash");
    archive(ar, obj.sig, "sig");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::PartialCertificateWire &obj) {
    archive(ar, obj.hash, "hash");
    archive(ar, obj.sig, "sig");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct QuorumCertificateWire {
  Hash justify{};
  ReplicaIDBitMap bmp{};
  xdr::xvector<Signature,MAX_REPLICAS> sigs{};

  QuorumCertificateWire() = default;
  template<typename _justify_T,
           typename _bmp_T,
           typename _sigs_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _justify_T>::value
                          && std::is_constructible<ReplicaIDBitMap, _bmp_T>::value
                          && std::is_constructible<xdr::xvector<Signature,MAX_REPLICAS>, _sigs_T>::value
                         >::type>
  explicit QuorumCertificateWire(_justify_T &&_justify,
                                 _bmp_T &&_bmp,
                                 _sigs_T &&_sigs)
    : justify(std::forward<_justify_T>(_justify)),
      bmp(std::forward<_bmp_T>(_bmp)),
      sigs(std::forward<_sigs_T>(_sigs)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::QuorumCertificateWire>
  : xdr_struct_base<field_ptr<::hotstuff::QuorumCertificateWire,
                              decltype(::hotstuff::QuorumCertificateWire::justify),
                              &::hotstuff::QuorumCertificateWire::justify>,
                    field_ptr<::hotstuff::QuorumCertificateWire,
                              decltype(::hotstuff::QuorumCertificateWire::bmp),
                              &::hotstuff::QuorumCertificateWire::bmp>,
                    field_ptr<::hotstuff::QuorumCertificateWire,
                              decltype(::hotstuff::QuorumCertificateWire::sigs),
                              &::hotstuff::QuorumCertificateWire::sigs>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::QuorumCertificateWire &obj) {
    archive(ar, obj.justify, "justify");
    archive(ar, obj.bmp, "bmp");
    archive(ar, obj.sigs, "sigs");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::QuorumCertificateWire &obj) {
    archive(ar, obj.justify, "justify");
    archive(ar, obj.bmp, "bmp");
    archive(ar, obj.sigs, "sigs");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct HotstuffBlockHeader {
  Hash parent_hash{};
  QuorumCertificateWire qc{};
  Hash body_hash{};

  HotstuffBlockHeader() = default;
  template<typename _parent_hash_T,
           typename _qc_T,
           typename _body_hash_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _parent_hash_T>::value
                          && std::is_constructible<QuorumCertificateWire, _qc_T>::value
                          && std::is_constructible<Hash, _body_hash_T>::value
                         >::type>
  explicit HotstuffBlockHeader(_parent_hash_T &&_parent_hash,
                               _qc_T &&_qc,
                               _body_hash_T &&_body_hash)
    : parent_hash(std::forward<_parent_hash_T>(_parent_hash)),
      qc(std::forward<_qc_T>(_qc)),
      body_hash(std::forward<_body_hash_T>(_body_hash)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::HotstuffBlockHeader>
  : xdr_struct_base<field_ptr<::hotstuff::HotstuffBlockHeader,
                              decltype(::hotstuff::HotstuffBlockHeader::parent_hash),
                              &::hotstuff::HotstuffBlockHeader::parent_hash>,
                    field_ptr<::hotstuff::HotstuffBlockHeader,
                              decltype(::hotstuff::HotstuffBlockHeader::qc),
                              &::hotstuff::HotstuffBlockHeader::qc>,
                    field_ptr<::hotstuff::HotstuffBlockHeader,
                              decltype(::hotstuff::HotstuffBlockHeader::body_hash),
                              &::hotstuff::HotstuffBlockHeader::body_hash>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::HotstuffBlockHeader &obj) {
    archive(ar, obj.parent_hash, "parent_hash");
    archive(ar, obj.qc, "qc");
    archive(ar, obj.body_hash, "body_hash");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::HotstuffBlockHeader &obj) {
    archive(ar, obj.parent_hash, "parent_hash");
    archive(ar, obj.qc, "qc");
    archive(ar, obj.body_hash, "body_hash");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct HotstuffBlockWire {
  HotstuffBlockHeader header{};
  xdr::opaque_vec<> body{};

  HotstuffBlockWire() = default;
  template<typename _header_T,
           typename _body_T,
           typename = typename
           std::enable_if<std::is_constructible<HotstuffBlockHeader, _header_T>::value
                          && std::is_constructible<xdr::opaque_vec<>, _body_T>::value
                         >::type>
  explicit HotstuffBlockWire(_header_T &&_header,
                             _body_T &&_body)
    : header(std::forward<_header_T>(_header)),
      body(std::forward<_body_T>(_body)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::HotstuffBlockWire>
  : xdr_struct_base<field_ptr<::hotstuff::HotstuffBlockWire,
                              decltype(::hotstuff::HotstuffBlockWire::header),
                              &::hotstuff::HotstuffBlockWire::header>,
                    field_ptr<::hotstuff::HotstuffBlockWire,
                              decltype(::hotstuff::HotstuffBlockWire::body),
                              &::hotstuff::HotstuffBlockWire::body>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::HotstuffBlockWire &obj) {
    archive(ar, obj.header, "header");
    archive(ar, obj.body, "body");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::HotstuffBlockWire &obj) {
    archive(ar, obj.header, "header");
    archive(ar, obj.body, "body");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct BlockFetchRequest {
  xdr::xvector<Hash> reqs{};

  BlockFetchRequest() = default;
  template<typename _reqs_T,
           typename = typename
           std::enable_if<std::is_constructible<xdr::xvector<Hash>, _reqs_T>::value
                         >::type>
  explicit BlockFetchRequest(_reqs_T &&_reqs)
    : reqs(std::forward<_reqs_T>(_reqs)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::BlockFetchRequest>
  : xdr_struct_base<field_ptr<::hotstuff::BlockFetchRequest,
                              decltype(::hotstuff::BlockFetchRequest::reqs),
                              &::hotstuff::BlockFetchRequest::reqs>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::BlockFetchRequest &obj) {
    archive(ar, obj.reqs, "reqs");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::BlockFetchRequest &obj) {
    archive(ar, obj.reqs, "reqs");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct BlockFetchResponse {
  xdr::xvector<HotstuffBlockWire> responses{};

  BlockFetchResponse() = default;
  template<typename _responses_T,
           typename = typename
           std::enable_if<std::is_constructible<xdr::xvector<HotstuffBlockWire>, _responses_T>::value
                         >::type>
  explicit BlockFetchResponse(_responses_T &&_responses)
    : responses(std::forward<_responses_T>(_responses)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::BlockFetchResponse>
  : xdr_struct_base<field_ptr<::hotstuff::BlockFetchResponse,
                              decltype(::hotstuff::BlockFetchResponse::responses),
                              &::hotstuff::BlockFetchResponse::responses>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::BlockFetchResponse &obj) {
    archive(ar, obj.responses, "responses");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::BlockFetchResponse &obj) {
    archive(ar, obj.responses, "responses");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct FetchBlocksV1 {
  static Constexpr const std::uint32_t program = 286331156;
  static Constexpr const char *program_name() { return "FetchBlocks"; }
  static Constexpr const std::uint32_t version = 1;
  static Constexpr const char *version_name() { return "FetchBlocksV1"; }

  struct fetch_t {
    using interface_type = FetchBlocksV1;
    static Constexpr const std::uint32_t proc = 1;
    static Constexpr const char *proc_name() { return "fetch"; }
    using arg_type = BlockFetchRequest;
    using arg_tuple_type = std::tuple<BlockFetchRequest>;
    using res_type = BlockFetchResponse;
    using res_wire_type = BlockFetchResponse;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.fetch(std::forward<A>(a)...)) {
      return c.fetch(std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<fetch_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDR_INVOKER> struct _xdr_client {
    _XDR_INVOKER _xdr_invoker_;
    template<typename...ARGS> _xdr_client(ARGS &&...args)
      : _xdr_invoker_(std::forward<ARGS>(args)...) {}

    template<typename..._XDR_ARGS> auto
    fetch(_XDR_ARGS &&..._xdr_args) ->
    decltype(_xdr_invoker_->template invoke<fetch_t,
                                            BlockFetchRequest>(
             std::forward<_XDR_ARGS>(_xdr_args)...)) {
      return _xdr_invoker_->template invoke<fetch_t,
                                            BlockFetchRequest>(
             std::forward<_XDR_ARGS>(_xdr_args)...);
    }
  };
};

struct VoteMessage {
  PartialCertificateWire vote{};
  ReplicaID voter{};

  VoteMessage() = default;
  template<typename _vote_T,
           typename _voter_T,
           typename = typename
           std::enable_if<std::is_constructible<PartialCertificateWire, _vote_T>::value
                          && std::is_constructible<ReplicaID, _voter_T>::value
                         >::type>
  explicit VoteMessage(_vote_T &&_vote,
                       _voter_T &&_voter)
    : vote(std::forward<_vote_T>(_vote)),
      voter(std::forward<_voter_T>(_voter)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::VoteMessage>
  : xdr_struct_base<field_ptr<::hotstuff::VoteMessage,
                              decltype(::hotstuff::VoteMessage::vote),
                              &::hotstuff::VoteMessage::vote>,
                    field_ptr<::hotstuff::VoteMessage,
                              decltype(::hotstuff::VoteMessage::voter),
                              &::hotstuff::VoteMessage::voter>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::VoteMessage &obj) {
    archive(ar, obj.vote, "vote");
    archive(ar, obj.voter, "voter");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::VoteMessage &obj) {
    archive(ar, obj.vote, "vote");
    archive(ar, obj.voter, "voter");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct ProposeMessage {
  HotstuffBlockWire proposal{};
  ReplicaID proposer{};

  ProposeMessage() = default;
  template<typename _proposal_T,
           typename _proposer_T,
           typename = typename
           std::enable_if<std::is_constructible<HotstuffBlockWire, _proposal_T>::value
                          && std::is_constructible<ReplicaID, _proposer_T>::value
                         >::type>
  explicit ProposeMessage(_proposal_T &&_proposal,
                          _proposer_T &&_proposer)
    : proposal(std::forward<_proposal_T>(_proposal)),
      proposer(std::forward<_proposer_T>(_proposer)) {}
};
} namespace xdr {
template<> struct xdr_traits<::hotstuff::ProposeMessage>
  : xdr_struct_base<field_ptr<::hotstuff::ProposeMessage,
                              decltype(::hotstuff::ProposeMessage::proposal),
                              &::hotstuff::ProposeMessage::proposal>,
                    field_ptr<::hotstuff::ProposeMessage,
                              decltype(::hotstuff::ProposeMessage::proposer),
                              &::hotstuff::ProposeMessage::proposer>> {
  template<typename Archive> static void
  save(Archive &ar, const ::hotstuff::ProposeMessage &obj) {
    archive(ar, obj.proposal, "proposal");
    archive(ar, obj.proposer, "proposer");
  }
  template<typename Archive> static void
  load(Archive &ar, ::hotstuff::ProposeMessage &obj) {
    archive(ar, obj.proposal, "proposal");
    archive(ar, obj.proposer, "proposer");
    xdr::validate(obj);
  }
};
} namespace hotstuff {

struct HotstuffProtocolV1 {
  static Constexpr const std::uint32_t program = 286331157;
  static Constexpr const char *program_name() { return "HotstuffProtocol"; }
  static Constexpr const std::uint32_t version = 1;
  static Constexpr const char *version_name() { return "HotstuffProtocolV1"; }

  struct vote_t {
    using interface_type = HotstuffProtocolV1;
    static Constexpr const std::uint32_t proc = 1;
    static Constexpr const char *proc_name() { return "vote"; }
    using arg_type = VoteMessage;
    using arg_tuple_type = std::tuple<VoteMessage>;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.vote(std::forward<A>(a)...)) {
      return c.vote(std::forward<A>(a)...);
    }
  };

  struct propose_t {
    using interface_type = HotstuffProtocolV1;
    static Constexpr const std::uint32_t proc = 2;
    static Constexpr const char *proc_name() { return "propose"; }
    using arg_type = ProposeMessage;
    using arg_tuple_type = std::tuple<ProposeMessage>;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.propose(std::forward<A>(a)...)) {
      return c.propose(std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<vote_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<propose_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDR_INVOKER> struct _xdr_client {
    _XDR_INVOKER _xdr_invoker_;
    template<typename...ARGS> _xdr_client(ARGS &&...args)
      : _xdr_invoker_(std::forward<ARGS>(args)...) {}

    template<typename..._XDR_ARGS> auto
    vote(_XDR_ARGS &&..._xdr_args) ->
    decltype(_xdr_invoker_->template invoke<vote_t,
                                            VoteMessage>(
             std::forward<_XDR_ARGS>(_xdr_args)...)) {
      return _xdr_invoker_->template invoke<vote_t,
                                            VoteMessage>(
             std::forward<_XDR_ARGS>(_xdr_args)...);
    }

    template<typename..._XDR_ARGS> auto
    propose(_XDR_ARGS &&..._xdr_args) ->
    decltype(_xdr_invoker_->template invoke<propose_t,
                                            ProposeMessage>(
             std::forward<_XDR_ARGS>(_xdr_args)...)) {
      return _xdr_invoker_->template invoke<propose_t,
                                            ProposeMessage>(
             std::forward<_XDR_ARGS>(_xdr_args)...);
    }
  };
};

}

#endif // !__XDR_HOTSTUFF_XDR_HOTSTUFF_H_INCLUDED__
