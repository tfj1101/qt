CREATE TABLE documents (
    doc_id INT PRIMARY KEY AUTO_INCREMENT,
    title VARCHAR(255) NOT NULL,       -- 批文标题
    content TEXT,                      -- 批文内容
    applicant_id INT NOT NULL,         -- 申请人ID（关联users表）
    created_at DATETIME DEFAULT NOW(), -- 申请时间
    current_status ENUM('draft', 'pending', 'approved', 'rejected') DEFAULT 'draft', -- 当前状态
    attachment_path VARCHAR(255),      -- 附件存储路径
    FOREIGN KEY (applicant_id) REFERENCES users(user_id)
);
